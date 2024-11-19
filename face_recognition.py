from picamera2 import Picamera2, Preview
import cv2

# Configuración de Picamera2
picam2 = Picamera2()
config = picam2.create_preview_configuration(main={"size": (640, 480)})
picam2.configure(config)
picam2.start()

# Carga el modelo de detección de rostros de Haarcascade
face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')

print("Presiona 'q' para salir.")

while True:
    # Captura una imagen desde la cámara
    frame = picam2.capture_array()

    # Convierte a escala de grises
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # Detecta rostros
    faces = face_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5, minSize=(30, 30))

    # Dibuja rectángulos alrededor de los rostros detectados
    for (x, y, w, h) in faces:
        cv2.rectangle(frame, (x, y), (x+w, y+h), (255, 0, 0), 2)

    frame_corrected = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)


    # Muestra el frame con los rostros detectados
    cv2.imshow('Reconocimiento Facial', frame_corrected)

    # Salir al presionar 'q'
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Detiene la cámara y cierra las ventanas
picam2.stop()
cv2.destroyAllWindows()
