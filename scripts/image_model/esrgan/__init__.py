from PIL import Image

import tensorflow as tf

# https://colab.research.google.com/github/tensorflow/hub/blob/master/examples/colab/image_enhancing.ipynb
def preprocess_image(image_path):
   """ Loads image from path and preprocesses to make it model ready
      Args:
         image_path: Path to the image file
   """
   image = tf.image.decode_image(tf.io.read_file(image_path))

   # If PNG, remove the alpha channel. The model only supports
   # images with 3 color channels.
   if image.shape[-1] == 4:
      image = image[...,:-1]
  
   size = (tf.convert_to_tensor(image.shape[:-1]) // 4) * 4
   
   image = tf.image.crop_to_bounding_box(image, 0, 0, size[0], size[1])
   image = tf.cast(image, tf.float32)

   return tf.expand_dims(image, 0)

def predict(model_path, input_image_path):
   input_image = preprocess_image(input_image_path)
   
   model = tf.keras.models.load_model(model_path)

   output_image = model.predict(input_image)

   output_image = tf.squeeze(output_image)
   output_image = tf.clip_by_value(output_image, 0, 255)
   output_image = tf.cast(output_image, tf.uint8)
   output_image = Image.fromarray(output_image.numpy())

   return output_image