# DCT Lossy Compression

# 1. Theory
## DCT Lossy Compression
이미지의 압축을 위한 방법입니다.
* 압축 순서
  1. 2D DCT에 사용될 8x8 DCT basis를 구한다.
  2. 입력 영상을 read 하여 graylevel로 변환한다.
  3. 이미지의 8x8 영역 f(i,j)를 8x8 DCT basis를 이용하여 주파스공간 8x8 F(u,v)로 변환한다.
## IDCT
DCT 방식으로 압축된 이미지를 원래 이미지로 되돌리는 작업입니다.
* IDCT는 DCT 압축 순서의 역순입니다.
