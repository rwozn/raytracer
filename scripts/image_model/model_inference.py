import os
import sys
import importlib

if len(sys.argv) < 5:
   raise ValueError("Provide model family name, model name, input image path and output image path as arguments");

model_family_name = sys.argv[1]
model_name = sys.argv[2]
input_image_path = sys.argv[3]
output_image_path = sys.argv[4]

module = importlib.import_module(model_family_name)

output_image = module.predict(os.path.join("models", model_family_name, model_name), input_image_path)

output_image.save(output_image_path)