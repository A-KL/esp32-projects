import time
import struct
from pySerialTransfer import pySerialTransfer as txfer

# pip3 install pySerialTransfer

logo_w = 115
logo_h = 32

logo = [ 
  0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xE0, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xF0,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x03, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x07, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xF8, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF8,
  0x00, 0x00, 0x00, 0x03, 0xC0, 0x00, 0x7E, 0x00, 0x00, 0x01, 0xE0, 0x00,
  0x7F, 0x0F, 0xF8, 0x00, 0x00, 0x00, 0x03, 0xC0, 0x00, 0xFE, 0x00, 0x00,
  0x01, 0xE0, 0x00, 0xFF, 0xEF, 0xF8, 0x00, 0x00, 0x00, 0x03, 0xC0, 0x00,
  0xFE, 0x00, 0x00, 0x01, 0xE0, 0x00, 0xFF, 0xFF, 0xF8, 0x00, 0x00, 0x00,
  0x03, 0xC0, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x7F, 0xFE, 0x7F,
  0xC0, 0x00, 0x00, 0x03, 0xC0, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x0F, 0x00,
  0x3F, 0xFE, 0x7F, 0xF8, 0x3F, 0xF1, 0xFB, 0xCF, 0xFC, 0xFE, 0xF3, 0xBC,
  0x3D, 0xEF, 0xE0, 0x1F, 0xFE, 0x7F, 0xFF, 0x7F, 0xFB, 0xFF, 0xDF, 0xFE,
  0xFE, 0xFF, 0xBC, 0x3D, 0xEF, 0xE0, 0x1F, 0xC6, 0xFF, 0xFF, 0x7F, 0xFB,
  0xFF, 0xDF, 0xFE, 0xFE, 0xFF, 0xBC, 0x3D, 0xEF, 0xE0, 0x0F, 0xE3, 0xC7,
  0xFE, 0x78, 0x7B, 0xC3, 0xDE, 0x1E, 0xF0, 0xFF, 0xBC, 0x3D, 0xEF, 0x00,
  0x07, 0xFF, 0x87, 0xFC, 0x78, 0x7B, 0xC3, 0xDE, 0x1E, 0xF0, 0xF8, 0x3C,
  0x3D, 0xEF, 0x00, 0x01, 0xFF, 0xFF, 0xF0, 0x00, 0x7B, 0xC3, 0xC0, 0x1E,
  0xF0, 0xF0, 0x3C, 0x3D, 0xEF, 0x00, 0x01, 0xF3, 0x7F, 0xE0, 0x3F, 0xFB,
  0xC3, 0xCF, 0xFE, 0xF0, 0xF0, 0x3C, 0x3D, 0xEF, 0x00, 0x03, 0xE3, 0x3F,
  0x80, 0x7F, 0xFB, 0xC3, 0xDF, 0xFE, 0xF0, 0xF0, 0x3C, 0x3D, 0xEF, 0x00,
  0x07, 0xE7, 0x3C, 0x00, 0x78, 0x7B, 0xC3, 0xDE, 0x1E, 0xF0, 0xF0, 0x3C,
  0x3D, 0xEF, 0x00, 0x07, 0xFF, 0xBE, 0x00, 0x78, 0x7B, 0xC3, 0xDE, 0x1E,
  0xF0, 0xF0, 0x3C, 0x3D, 0xEF, 0x00, 0x07, 0xFF, 0xFE, 0x00, 0x78, 0x7B,
  0xC3, 0xDE, 0x1E, 0xF0, 0xF0, 0x3C, 0x3D, 0xEF, 0x00, 0x0F, 0xFF, 0xFE,
  0x00, 0x7F, 0xFB, 0xFF, 0xDF, 0xFE, 0xF0, 0xF0, 0x3F, 0xFD, 0xEF, 0xE0,
  0x0F, 0xFF, 0xFF, 0x00, 0x7F, 0xFB, 0xFF, 0xDF, 0xFE, 0xF0, 0xF0, 0x3F,
  0xFD, 0xEF, 0xE0, 0x0F, 0xF9, 0xFF, 0x00, 0x3E, 0x79, 0xF9, 0xCF, 0x9E,
  0xF0, 0xF0, 0x1F, 0x3D, 0xE7, 0xE0, 0x1F, 0xF1, 0xFF, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x80, 0xFF,
  0x00, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0,
  0x1C, 0x00, 0x7F, 0x00, 0x7F, 0xFF, 0xFF, 0xFF, 0xFE, 0xB4, 0x6D, 0x88,
  0x8D, 0x18, 0xE0, 0x00, 0x00, 0x1F, 0x00, 0x7F, 0xFF, 0xFF, 0xFF, 0xFE,
  0x95, 0xAD, 0x7D, 0xB5, 0x77, 0xE0, 0x00, 0x00, 0x0F, 0x00, 0x7F, 0xFF,
  0xFF, 0xFF, 0xFE, 0xA5, 0xAD, 0x9D, 0x8D, 0x19, 0xE0, 0x00, 0x00, 0x06,
  0x00, 0x7F, 0xFF, 0xFF, 0xFF, 0xFE, 0xB5, 0xAD, 0xED, 0xB5, 0x7E, 0xE0 ]

# Doesn't work here but works on arduino
logo_v2 = [
  0b00110000, 0b01110000, 0b11110000, 0b11110000,
  0b01100000, 0b01100000, 0b11111000, 0b10011110,
  0b11111111, 0b01111111, 0b01111000, 0b11100000,
  0b11100000, 0b11000000, 0b11000000, 0b11000000,
  0b00000000, 0b01100000, 0b01111000, 0b01111101,
  0b00111111, 0b00111011, 0b00011101, 0b00011111,
  0b00111101, 0b01111011, 0b11111111, 0b11110011,
  0b00000001, 0b00000001, 0b00000000, 0b00000000 ]

# Works here
logo_v3 = [
  0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000 ]

# works here
logo_v4 = [
  0b11111111, 0b11111111, 0b11111111, 0b11111111,
  0b10000000, 0b00000000, 0b00000000, 0b00000001,
  0b10000000, 0b00000000, 0b00000000, 0b00000001,
  0b10000000, 0b00000000, 0b00000000, 0b00000001,
  0b10000000, 0b00000000, 0b00000000, 0b00000001,
  0b10000000, 0b00000000, 0b00000000, 0b00000001,
  0b10000000, 0b00000000, 0b00000000, 0b00000001,
  0b10000000, 0b00000000, 0b00000000, 0b00000001,
  0b10000000, 0b00000000, 0b00000000, 0b00000001,
  0b10000000, 0b00000000, 0b00000000, 0b00000001,
  0b10000000, 0b00000000, 0b00000000, 0b00000001,
  0b10000000, 0b00000000, 0b00000000, 0b00000001,
  0b10000000, 0b00000000, 0b00000000, 0b00000001,
  0b10000000, 0b00000000, 0b00000000, 0b00000001,
  0b10000000, 0b00000000, 0b00000000, 0b00000001,
  0b10000000, 0b00000000, 0b00000000, 0b00000001,

  0b10000000, 0b00000000, 0b00000000, 0b00000001,
  0b10000000, 0b00000000, 0b00000000, 0b00000001,
  0b10000000, 0b00000000, 0b00000000, 0b00000001,
  0b10000000, 0b00000000, 0b00000000, 0b00000001,
  0b10000000, 0b00000000, 0b00000000, 0b00000001,
  0b10000000, 0b00000000, 0b00000000, 0b00000001,
  0b10000000, 0b00000000, 0b00000000, 0b00000001,
  0b10000000, 0b00000000, 0b00000000, 0b00000001,
  0b10000000, 0b00000000, 0b00000000, 0b00000001,
  0b10000000, 0b00000000, 0b00000000, 0b00000001,
  0b10000000, 0b00000000, 0b00000000, 0b00000001,
  0b10000000, 0b00000000, 0b00000000, 0b00000001,
  0b10000000, 0b00000000, 0b00000000, 0b00000001,
  0b10000000, 0b00000000, 0b00000000, 0b00000001,
  0b10000000, 0b00000000, 0b00000000, 0b00000001,
  0b11111111, 0b11111111, 0b11111111, 0b11111111 ]

def display_draw(connection, display, x, y, w, h, data):
    
    header = struct.pack("<BHHHH", display, x, y, w, h)

    size_header = len(header)
    size_body = len(data)

    size = connection.tx_struct_obj(header)

    print(f'Header size: {size}, {size_header} data size in bytes: {size_body}')

    connection.send(size)

    data_per_packet = 200
    numPackets = (size_body + data_per_packet - 1) // data_per_packet

    for i in range(numPackets):
        fileIndex = i * data_per_packet
        dataLen = min(data_per_packet, size_body - fileIndex)
        
        batch = data[fileIndex:fileIndex+dataLen]
        
        size = connection.tx_obj(fileIndex, val_type_override='h')
        print(f'Data header size: ({fileIndex}) {size}')

        size = connection.tx_struct_obj(bytes(batch), start_pos=size)
        # size = link.tx_obj(batch, start_pos=size)
        print(f'Total data batch size: {size}')
        
        connection.send(size, 1)
        # time.sleep(0.05)

if __name__ == '__main__':
    try:
        link = txfer.SerialTransfer('/dev/cu.usbserial-A5069RR4')
        
        if not link.open():
            raise RuntimeError(f'Could not open serial port: {link.port_name}')

        # Match the reset normally triggered when the VS Code serial monitor opens.
        link.connection.setDTR(False)
        time.sleep(0.1)
        link.connection.setDTR(True)
        time.sleep(2) # allow the Arduino bootloader and firmware to start
        
        while True:
            # display_draw(link, 0, 0, 0, logo_w, logo_h, logo)
            display_draw(link, 0, 0, 0, 32, 32, logo_v4)
            time.sleep(1)
            display_draw(link, 0, 32, 0, 32, 32, logo_v4)
            time.sleep(1)
            display_draw(link, 0, 64, 0, 32, 32, logo_v4)
            time.sleep(1)
            continue
            link.close()
            break
            # send_size = 0
            
            ###################################################################
            # Send a list
            ###################################################################
            # list_ = [1, 3]
            # list_size = link.tx_obj(list_)
            # send_size += list_size
            
            ###################################################################
            # Send a string
            ###################################################################
            # str_ = 'hello'
            # str_size = link.tx_obj(str_, send_size) - send_size
            # send_size += str_size
            
            ###################################################################
            # Send a float
            ###################################################################
            # float_ = 5.234
            # float_size = link.tx_obj(float_, send_size) - send_size
            # send_size += float_size
            
            ###################################################################
            # Transmit all the data to send in a single packet
            ###################################################################
            # link.send(send_size)
            
            ###################################################################
            # Wait for a response and report any errors while receiving packets
            ###################################################################
            while not link.available():
                # A negative value for status indicates an error
                if link.status.value < 0:
                    if link.status == txfer.Status.CRC_ERROR:
                        print('ERROR: CRC_ERROR')
                    elif link.status == txfer.Status.PAYLOAD_ERROR:
                        print('ERROR: PAYLOAD_ERROR')
                    elif link.status == txfer.Status.STOP_BYTE_ERROR:
                        print('ERROR: STOP_BYTE_ERROR')
                    else:
                        print('ERROR: {}'.format(link.status.name))
            
            ###################################################################
            # Parse response list
            ###################################################################
            # rec_list_  = link.rx_obj(obj_type=type(list_),
            #                          obj_byte_size=list_size,
            #                          list_format='i')
            
            ###################################################################
            # Parse response string
            ###################################################################
            # rec_str_   = link.rx_obj(obj_type=type(str_),
            #                          obj_byte_size=str_size,
            #                          start_pos=list_size)
            
            ###################################################################
            # Parse response float
            ###################################################################
            # rec_float_ = link.rx_obj(obj_type=type(float_),
            #                          obj_byte_size=float_size,
            #                          start_pos=(list_size + str_size))
            
            ###################################################################
            # Display the received data
            ###################################################################
            # print('SENT: {} {} {}'.format(list_, str_, float_))
            # print('RCVD: {} {} {}'.format(rec_list_, rec_str_, rec_float_))
            # print(' ')
    
    except KeyboardInterrupt:
        try:
            link.close()
        except:
            pass
    
    except:
        import traceback
        traceback.print_exc()
        
        try:
            link.close()
        except:
            pass