from PIL import Image


with open('Observation', 'rb') as observation:
    data: bytes = observation.read()
    image: 'Image' = Image.frombytes('L', (128, 128), bytes([byte for byte in data]))
    image.save('Observation.png', 'PNG')
