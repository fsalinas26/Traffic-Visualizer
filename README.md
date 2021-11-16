# Traffic-Visualizer
INRIX Hackathon 2021 


![](https://i.gyazo.com/3ef97735287b616700100ee7527b6e4c.gif)

# Process
## 1. Initialize the Quadkey to fetch from the INRIX API

![](https://i.gyazo.com/ff38f1686344f40acbb807210dba6fac.png)

## 2. Program wil fetch 8 256x256px traffic tiles within the quadkey depth and proceed to stitch them together into a 1024x1024px image.

![](https://i.gyazo.com/7c2c287b847cc0711b4763a04dcaf8f8.png)

## 3. Once the stitched image is complete, we will use OpenCV to filter our image and extract our contours into a vector of coordinates that we can map. 
![](https://i.gyazo.com/708580fe0177bc85dacb802ee8e2bb39.png)

## 4. Voilà
We can now analyze and observe our map in much higher quality. 
![](https://i.gyazo.com/5d9203bf579a0f8394205ac673c52538.png)
