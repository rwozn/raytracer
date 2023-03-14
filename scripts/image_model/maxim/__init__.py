import os

from PIL import Image

import numpy as np
import tensorflow as tf

# https://github.com/sayakpaul/maxim-tf/blob/main/notebooks/inference-dynamic-resize.ipynb
from utils.configs import MAXIM_CONFIGS
from utils.create_maxim_model import Model

def mod_padding_symmetric(image, factor=64):
   """Padding the image to be divided by factor."""
   height, width = image.shape[0], image.shape[1]
   
   height_pad, width_pad = ((height + factor) // factor) * factor, (
      (width + factor) // factor
   ) * factor
   
   padh = height_pad - height if height % factor != 0 else 0
   padw = width_pad - width if width % factor != 0 else 0
   
   image = tf.pad(
      image, [(padh // 2, padh // 2), (padw // 2, padw // 2), (0, 0)], mode="REFLECT"
   )

   return image

def make_shape_even(image):
   """Pad the image to have even shapes."""
   height, width = image.shape[0], image.shape[1]

   padh = 1 if height % 2 != 0 else 0
   padw = 1 if width % 2 != 0 else 0

   image = tf.pad(image, [(0, padh), (0, padw), (0, 0)], mode="REFLECT")

   return image

def process_image(image: Image):
   input_image = np.asarray(image) / 255

   height, width = input_image.shape[0], input_image.shape[1]

   # Padding images to have even shapes
   input_image = make_shape_even(input_image)
   height_even, width_even = input_image.shape[0], input_image.shape[1]

   # padding images to be multiplies of 64
   input_image = mod_padding_symmetric(input_image, factor=64)
   input_image = tf.expand_dims(input_image, axis=0)

   return input_image, height, width, height_even, width_even

def create_model(input_image, model_path):
   variant = model_path.split(os.path.sep)[-1].split('_')[0].upper()
   
   configs = MAXIM_CONFIGS.get(variant)
   configs.update({
      "variant": "S-2",
      "use_bias": True,
      "num_outputs": 3,
      "dropout_rate": 0.0,
      "num_supervision_scales": 3
   })
   
   # From https://github.com/google-research/maxim/blob/main/maxim/run_eval.py#L45-#L61
   configs.update({"input_resolution": (input_image.shape[1], input_image.shape[2])})
   
   model = Model(**configs)
   model.set_weights(tf.keras.models.load_model(model_path).get_weights())

   return model

# Based on https://github.com/google-research/maxim/blob/main/maxim/run_eval.py
def infer(model_path: str, image_path: str):
   image = Image.open(image_path).convert("RGB")

   preprocessed_image, height, width, height_even, width_even = process_image(image)
   
   model = create_model(preprocessed_image, model_path)

   predictions = model.predict(preprocessed_image)

   if isinstance(predictions, list):
      predictions = predictions[-1]
      if isinstance(predictions, list):
         predictions = predictions[-1]

   predictions = np.array(predictions[0], np.float32)

   new_height, new_width = predictions.shape[0], predictions.shape[1]

   h_start = new_height // 2 - height_even // 2
   h_end = h_start + height

   w_start = new_width // 2 - width_even // 2
   w_end = w_start + width

   predictions = predictions[h_start:h_end, w_start:w_end, :]

   return np.array(np.clip(predictions, 0, 1))

def predict(model_path, input_image_path):
   output_image = infer(model_path, input_image_path)

   output_image = np.uint8(output_image * 255)
   output_image = Image.fromarray(output_image)

   return output_image
