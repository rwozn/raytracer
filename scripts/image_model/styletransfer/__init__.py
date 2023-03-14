import os

import numpy as np
import tensorflow as tf

from PIL import Image

# https://github.com/sayakpaul/Adventures-in-TensorFlow-Lite/blob/master/Style_Transfer_Demo_InceptionV3.ipynb
# Function to load an image from a file, and add a batch dimension.
def load_style_image(image_path):
   img = tf.io.read_file(image_path)

   img = tf.io.decode_image(img, channels=3)
   img = tf.image.convert_image_dtype(img, tf.float32)
   img = img[tf.newaxis, :]

   return img

# Function to load an image from a file, and add a batch dimension.
def load_content_image(image_path):
   image = Image.open(image_path)
   image = np.array(image)

   if image.shape[-1] == 4:
      image = Image.fromarray(image)
      image = image.convert("RGB")
      
      image = np.array(image)
      image = tf.convert_to_tensor(image)
      image = tf.image.convert_image_dtype(image, tf.float32)
      image = image[tf.newaxis, :]

      return image
   
   if image.shape[-1] == 3:
      image = tf.convert_to_tensor(image)
      image = tf.image.convert_image_dtype(image, tf.float32)
      image = image[tf.newaxis, :]

      return image
   
   if image.shape[-1] == 1:
      raise ValueError("Grayscale images not supported! Please try with RGB or RGBA images.")

# Function to pre-process by resizing an central cropping it.
def preprocess_image(image, target_dim):
  # Resize the image so that the shorter dimension becomes 256px.
  shape = tf.cast(tf.shape(image)[1:-1], tf.float32)
  short_dim = min(shape)
  scale = target_dim / short_dim
  new_shape = tf.cast(shape * scale, tf.int32)
  image = tf.image.resize(image, new_shape)

  # Central crop the image.
  image = tf.image.resize_with_crop_or_pad(image, target_dim, target_dim)

  return image

def predict(model_path, input_image_path):
   input_image = load_content_image(input_image_path)

   style_image = load_style_image(os.path.join(os.path.dirname(model_path), "styles", "style.png"))
   style_image = preprocess_image(style_image, 256)

   model = tf.keras.models.load_model(model_path)

   # Stylize image.
   predictions = model(input_image, style_image)

   output_image = np.array(predictions[0][0], np.float32)
   output_image = np.clip(output_image, 0, 1)
   output_image = np.uint8(output_image * 255)
   output_image = Image.fromarray(output_image)

   return output_image