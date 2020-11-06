# Error Diffusion Dithering

# Theory
## Dithering
하나의 픽셀이 가지는 밝기의 단계가 많을 수록 이미지의 크기가 커지는데
이를 1bit(두 단계)로 나타낼 수 있도록 하여 용량을 줄이는 일종의 양자화 과정
ex) 8bit의 픽셀을 가진 이미지가 있으면 dithering을 통해 이를 1bit의 픽셀로 표현할 수 있다. 

## Error Diffusion Dithering
밝기값 0, 255 중 픽셀값과 가까운 밝기값과의 오차를 계산하여
주변 픽셀로 전파하는 방법의 dithering 방식이다. 

# Code
