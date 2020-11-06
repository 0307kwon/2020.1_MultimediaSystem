# Error Diffusion Dithering

# 1. Theory
## Dithering
하나의 픽셀이 가지는 밝기의 단계가 많을 수록 이미지의 크기가 커지는데
이를 1bit(두 단계)로 나타낼 수 있도록 하여 용량을 줄이는 일종의 양자화 과정  
ex) 8bit의 픽셀을 가진 이미지가 있으면 dithering을 통해 이를 1bit의 픽셀로 표현할 수 있다. 

## Error Diffusion Dithering
밝기값 0, 255 중 픽셀값과 가까운 밝기값과의 오차를 계산하여
주변 픽셀로 전파하는 방법의 dithering 방식이다. 

# 2. Code
## void make_edd(Mat* output)
Mat 포인터 output을 인자로 받아 error diffusion dithering을 수행하고
결과를 output에 저장합니다.
1. (line 22 ~ 30)  
output 인자를 바로 수정하는 것이 아닌
동일한 크기의 map[][]을 선언하여 output을 map에 복사한 후,
map에서 계산하고 map을 수정합니다.
```c
for (y = 0; y < output->rows; y++)
	{
		map[y] = (double*)malloc(sizeof(double) * output->cols);
	}
	for (y = 0; y < output->rows; y++) {
		for (x = 0; x < output->cols; x++) {
			map[y][x] = output->at<uchar>(y, x);
		}
	}
```

2. (line 33 ~ 49)  
오차확산을 진행합니다. 
```c
//과정 1 : 오차확산 (좌우, 아래 1픽셀씩은 계산하지않음, x=1, y=0 부터 시작)
	//반드시 for 문은 y부터 시작해야 순서대로 오차확산됨
	for (y = 0; y < output->rows - 1; y++) {
		for (x = 1; x < output->cols - 1; x++) {
			if (map[y][x] > 127) {
				e = map[y][x] - 255;
			}
			else {
				e = map[y][x];
			}

			map[y][x + 1] = map[y][x + 1] + e * 7 / 16.0;
			map[y + 1][x + 1] = map[y + 1][x + 1] + e * 1 / 16.0;
			map[y + 1][x] += e * 5 / 16.0;
			map[y + 1][x - 1] += e * 3 / 16.0;
		}
	}
```
3. (line 51 ~ 61)  
오차를 확산한 최종값이 255에 가까우면 255로 바꾸고 (1bit 중 1의 값)
0에 가까우면 0으로 바꿔줍니다. (1bit 중 0의 값)
```c
//과정 2 : 1bit로 변환
	for (y = 0; y < output->rows; y++) {
		for (x = 0; x < output->cols; x++) {
			if (map[y][x] > 127) {
				output->at<uchar>(y, x) = 255;
			}
			else {
				output->at<uchar>(y, x) = 0;
			}
		}
	}
```
# 3. Result
color image -> gray scale image -> dithered image 순서로 진행하였습니다.

## color image
<img src="./image01.png" width=500px>



