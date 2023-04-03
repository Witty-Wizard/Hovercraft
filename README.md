# Hovercraft

This hovercraft was designed using 2 2400 emax 2207 motors , xylo 40A ESC and ESP32.

The Esp32 is responsible for decoding the sbus signal from the reciever and then produce the output signals for motor. The main reason of using ESP32 was so that we could reverse the direction of motor using 3d mode which is run over the dshot protocol. This nabled the hovercraft to go in the reverse direction if required even if that was with very less efficiency.