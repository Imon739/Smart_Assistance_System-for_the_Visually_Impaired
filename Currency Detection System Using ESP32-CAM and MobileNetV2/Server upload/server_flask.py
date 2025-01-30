from flask import Flask, request
import tensorflow as tf
import numpy as np
from PIL import Image
import io
import os
# import tensorflow as t
print("Current working directory:", os.getcwd())  # Debugging current directory

app = Flask(__name__)

# Load the MobileNetV2 model
model = tf.keras.models.load_model('C:\\Users\\88016\\Desktop\\Project\\Server upload\\currency_model.h5')

# Define class labels
CLASS_NAMES = {
    0: "10 Taka",
    1: "100 Taka",
    2: "50 Taka"
}

@app.route('/predict', methods=['POST'])
def predict():
    try:
        # Check if the request contains an image
        if 'image' in request.files:
            file = request.files['image']
            img = Image.open(file.stream)
        elif request.data:
            img = Image.open(io.BytesIO(request.data))
        else:
            return "No image provided", 400

        # Preprocess the image
        img = img.resize((224, 224))  # Resize to MobileNetV2 input size
        img_array = np.array(img, dtype=np.float32) / 255.0  # Normalize to [0, 1]
        img_array = np.expand_dims(img_array, axis=0)  # Add batch dimension

        # Run inference
        predictions = model.predict(img_array)
        predicted_class = np.argmax(predictions)

        # Get the class name
        class_name = CLASS_NAMES.get(predicted_class, "Unknown")
        print("Predicted class:", class_name)
        return class_name  # Return just the class name

    except Exception as e:
        return str(e), 500

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
