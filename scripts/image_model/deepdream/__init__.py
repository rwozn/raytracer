import os
import numpy as np
import tensorflow as tf

from PIL import Image

#https://colab.research.google.com/github/tensorflow/docs/blob/master/site/en/tutorials/generative/deepdream.ipynb
def random_roll(img, maxroll):
  # Randomly shift the image to avoid tiled boundaries.
  shift = tf.random.uniform(shape=[2], minval=-maxroll, maxval=maxroll, dtype=tf.int32)
  img_rolled = tf.roll(img, shift=shift, axis=[0, 1])
  return shift, img_rolled

def calc_loss(img, model):
   # Pass forward the image through the model to retrieve the activations.
   # Converts the image into a batch of size 1.
   img_batch = tf.expand_dims(img, axis=0)
   layer_activations = model(img_batch)
   if len(layer_activations) == 1:
      layer_activations = [layer_activations]

   losses = []
   for act in layer_activations:
      loss = tf.math.reduce_mean(act)
      losses.append(loss)

   return tf.reduce_sum(losses)

class TiledGradients(tf.Module):
   def __init__(self, model):
      self.model = model

   @tf.function(
      input_signature=(
         tf.TensorSpec(shape=[None, None, 3], dtype=tf.float32),
         tf.TensorSpec(shape=[2], dtype=tf.int32),
         tf.TensorSpec(shape=[], dtype=tf.int32))
   )
   def __call__(self, img, img_size, tile_size=512):
      shift, img_rolled = random_roll(img, tile_size)

      # Initialize the image gradients to zero.
      gradients = tf.zeros_like(img_rolled)
      
      # Skip the last tile, unless there's only one tile.
      xs = tf.range(0, img_size[1], tile_size)[:-1]
      if not tf.cast(len(xs), bool):
         xs = tf.constant([0])
      
      ys = tf.range(0, img_size[0], tile_size)[:-1]
      if not tf.cast(len(ys), bool):
         ys = tf.constant([0])

      # Calculate the gradients for this tile.
      with tf.GradientTape() as tape:
         for x in xs:
            for y in ys:
                  # This needs gradients relative to `img_rolled`.
                  # `GradientTape` only watches `tf.Variable`s by default.
                  tape.watch(img_rolled)

         # Extract a tile out of the image.
         img_tile = img_rolled[y:y+tile_size, x:x+tile_size]
         loss = calc_loss(img_tile, self.model)

         # Update the image gradients for this tile.
         gradients = gradients + tape.gradient(loss, img_rolled)

      # Undo the random shift applied to the image and its gradients.
      gradients = tf.roll(gradients, shift=-shift, axis=[0, 1])

      # Normalize the gradients.
      gradients /= tf.math.reduce_std(gradients) + 1e-8 

      return gradients

def run_deep_dream_with_octaves(img, get_tiled_gradients, steps_per_octave=100, step_size=0.01, octaves=range(-2,3), octave_scale=1.3):
   base_shape = tf.shape(img)
   
   img = tf.keras.utils.img_to_array(img)
   img = tf.keras.applications.inception_v3.preprocess_input(img)

   initial_shape = img.shape[:-1]
   
   img = tf.image.resize(img, initial_shape)

   for octave in octaves:
      # Scale the image based on the octave
      new_size = tf.cast(tf.convert_to_tensor(base_shape[:-1]), tf.float32) * (octave_scale ** octave)
      new_size = tf.cast(new_size, tf.int32)

      img = tf.image.resize(img, new_size)

      for step in range(steps_per_octave):
         gradients = get_tiled_gradients(img, new_size)
         img = img + gradients * step_size
         img = tf.clip_by_value(img, -1, 1)
   
   img = 255 * (img + 1) / 2
   img = tf.cast(img, tf.uint8)
   img = tf.image.resize(img, base_shape[:-1])
   img = tf.image.convert_image_dtype(img / 255, dtype=tf.uint8)

   return img

def get_dream_model(model_path):
   base_model = tf.keras.applications.InceptionV3(include_top=False, weights=os.path.join(model_path, "inception_v3_weights.h5"))

   # Maximize the activations of these layers
   names = ["mixed3", "mixed5"]
   layers = [base_model.get_layer(name).output for name in names]

   # Create the feature extraction model
   dream_model = tf.keras.Model(inputs=base_model.input, outputs=layers)

   return dream_model

def predict(model_path, input_image_path):
   input_image = np.array(Image.open(input_image_path))

   output_image = run_deep_dream_with_octaves(img=input_image, get_tiled_gradients=TiledGradients(get_dream_model(model_path)), step_size=0.01)

   output_image = Image.fromarray(output_image.numpy())

   return output_image