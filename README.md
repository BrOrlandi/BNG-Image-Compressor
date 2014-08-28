BNG Image Compressor
=====================

Authors
-----------
- Bruno Henrique Orlandi		*brorlandi@gmail.com*
- Guilherme Barranco Piva		*gui.piva@hotmail.com*
- Nihey Luz Takizawa			*nihey.takizawa@gmail.com*


Repositório
-------------

- CodeBlocks v13.12 project
- Windows
- TAB = 4 spaces

Project
---------
![Projeto](https://raw.githubusercontent.com/BrOrlandi/BNG-Image-Compressor/master/Projeto.png "Projeto")

Development Steps
-----------
![Fases](https://raw.githubusercontent.com/BrOrlandi/BNG-Image-Compressor/master/Fases.png "Fases")

Interfaces
-----------
![Interfaces](https://raw.githubusercontent.com/BrOrlandi/BNG-Image-Compressor/master/Interfaces.png "Interfaces")

About
-------

This program uses 3 methods to compress images, the discrete cosine transform (DCT), run length encoding and the Huffman's encoding.
For the DCT quantization we used the JPEG table.
The files are compressed with about 200 kb more than JPEG files. The files after decompress may have some noises at high frequency regions caused by the quantization.

We use multithreading to improve the algorithm performance at the DCT algorithm.

Some results execution:

Image: 1600x1200
Compression:
- Single thread: 	30.0 seconds
- Multithread: 		8.6 seconds
- Speedup: 			3.49

Decompression:
- Single thread: 	48.0 seconds
- Multithread: 		9.2 seconds
- Speedup: 			5.22

File Size:
- Before compression: 		5.626 KB
- After compression: 		413 KB
- With JPEG compression: 	348 KB

--------

Image: 3624x2448
Compression:
- Single thread: 	2 minutes, 9 seconds = 129 seconds
- Multithread: 		35 seconds
- Speedup: 			3.69

Decompression:
- Single thread: 	3 minutes e 20 seconds = 200 seconds
- Multithread: 		38 seconds
- Speedup: 			5.26

File Size:
- Before compression: 		23.410 MB
- After compression: 		2.487 MB
- With JPEG compression: 	2.230 MB

----

Number of Threads: 32

CPU: Intel Core i5, 3.0 GHz
Memory: 8 Gb
OS: Windows 8.1 - 64 bits