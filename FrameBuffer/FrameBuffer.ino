/*
 Name:		FrameBuffer.ino
 Created:	10/10/2020 6:56:22 PM
 Author:	horin
*/

#include <Arduboy2.h>

Arduboy2 arduboy;

// the setup function runs once when you press reset or power the board
void setup() {
	arduboy.begin();
	arduboy.clear();
	arduboy.setFrameRate(60);
	arduboy.initRandomSeed();
}

// the loop function runs over and over again until power down or reset
void loop() {
	if (arduboy.nextFrame()) {
		arduboy.clear();
		
		auto Buffer = arduboy.getBuffer();

		//!< ---------------------------------------------------
		//!< �ŏ���8bit��1��ڂ�8�s(0-7)���A����8bit��2��ڂ�8�s���A... 127��ڂ܂ōs������
		//!< �����8�s����(8-15)��1��ڂ���n�܂�A�ȉ����l�̌J��Ԃ�
		//!< 64/8 = 8��J��Ԃ���鎖�ɂȂ�
		//!< ---------------------------------------------------
		//!< (0 - 7 �s��)
		//!< [0] [1] ... [127]
		//!<  0   8       1016(=127x8)
		//!<  1   9       1017
		//!<  ... ...     ...
		//!<  7   15      1023

		//!< (8 - 15 �s��)
		//!< [128] ... [255]

		//!< ...

		//!< (56 - 63 �s��)
		//!< [896(=128x7)] ... [1023]
		//!< ---------------------------------------------------

#if 1
		//!< (0 - 7 �s��) ����8x8�ɎO�p�`��`�悷��
		//!< *-------
		//!< **------
		//!< ***-----
		//!< ****----
		//!< *****---
		//!< ******--
		//!< *******-
		//!< ********
		Buffer[0] = 0xff; //!< 1111 1111
		Buffer[1] = 0xfe; //!< 1111 1110
		Buffer[2] = 0xfc; //!< 1111 1100
		Buffer[3] = 0xf8; //!< 1111 1000
		Buffer[4] = 0xf0; //!< 1111 0000		
		Buffer[5] = 0xe0; //!< 1110 0000
		Buffer[6] = 0xc0; //!< 1100 0000
		Buffer[7] = 0x80; //!< 1000 0000

		constexpr auto Width = Arduboy2::width();
		//!< (8 - 15 �s��) �����_��
		for (auto i = 0; i < Width; ++i) { Buffer[i + Width] = rand() % 0xff; }

		//!< (16 - 23 �s��) ���h��
		for (auto i = 0; i < Width; ++i) { Buffer[i + Width * 2] = 0xff; }

		//!< (24 - 31 �s��) ���h��
		for (auto i = 0; i < Width; ++i) { Buffer[i + Width * 3] = 0x00; }

		//!< (32 - 39 �s��) �s���͗l
		for (auto i = 0; i < Width; ++i) { Buffer[i + Width * 4] = (i & 1) ? 0xaa : 0x55; } //!< 1010 1010 or 0101 0101
		
		//!< (40 - 47 �s��) ����
		for (auto i = 0; i < Width; ++i) { Buffer[i + Width * 5] = 0xaa; } //!< 1010 1010

		//!< (48 - 55 �s��) �c��
		for (auto i = 0; i < Width; ++i) { Buffer[i + Width * 6] = (i & 1) ? 0xff : 0x00; } //!< 1111 1111 or 0000 0000

		//!< (56 - 63 �s��)
		for (auto i = 0; i < Width; ++i) { Buffer[i + Width * 7] = (i > Width / 2) ? 0xf0 : 0x0f; } //!< 1111 0000 or 0000 1111

		arduboy.display();
#else
		//!< �����_��
		for (auto i = 0; i < Arduboy2::width() * Arduboy2::height() / 8; ++i) {
			arduboy.paint8Pixels(rand() % 0xff);
		}
#endif
	}
}