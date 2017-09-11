# PCA(Principal Components Analysis)
To recognize human face, I use the PCA. For this I must calculate eigenvectors and eigenvalues from given training image set. Then, test the face recognition accuray while coordinating the numbers of eigenvectors(eigenface). The maximum number of possible eigenvectors is ten.

## Background
 - This is a way of **identifying patterns in data**. The PCA can reduce the number of dimension because all dimensions’s data do not have important values except some special ones among data set without much loss of information. That is possible by eigenvectors that represent the data set best and eigenvalue corresponding to data set.
 ![pca](/assets/pca.jpg)

- For example, the graph on the top shows the distribution of values along a2 is wide, while the distribution gets narrow approaching a1. This shows that data is **sensitive along with a2 axis** but **insensitive along with a1 axis**. So it is possible to represent data using **only a2 axis and ignore some errors.**

# Train and Test image
- From here, training people are used 6 people and each person's face is prepared 8 images. Fase photos are little different such as face side or sight direction.

![face_example](/assets/face_example.JPG)

- Test images are different with training images. For example someone takes off the glassess.
![test_face](/assets/test_face.JPG)


# Specification
- Cross platform(get eigen values from Matlab but implement from Visual C++)
- STL(vector and pair) and lamda function used for experimentally.

# Design detail
 In my program, the PCA is divided for 7 steps. But it takes a long time to calculate the eigenvectors on visual studio(VS), **so I use the eigenvectors obtained through the matlab in advance(Once in Step 1).** And the face recognition rate was measured by adjusting the number of eigenvector(s) from 1 to 10.

Each stage goes as following:
### Step 1: Calculate the eigenvectors from matlab by training data set.
-	 This process is same with step 2 and step 3 and additionally executes the eigen function with covariance obtained from normalized training set matrix with transposed the matrix. Then the most dominant eigenvectors output in format of text file. (The dominant eigenvectors are located in the end of matrix sequentially)

### Step 2: Read training data set and calculate the average vector. (From here, rest of the steps is proceeded in VS)
-	 Because all data should be normalization under the mean of training data.

### Step 3: Normalize training data set though subtracting the mean.
-	It means that training data set is rearranged based on the median value.

### Step 4: Choosing dominant eigenvectors according to its coordinated number.
-	 According to the experimental environment, program will read eigenvectors. The dominant eigenvector means the axis that makes the variance large in a given data set. So remaining axes are ignored, since they have small eigenvalue in dominant axis.

### Step 5: Represent training vector by projection transposed set of eigenvectors.
-	 The selected feature vectors are component to make new data set. For converting new data set, all old data set is projected to feature vectors. Now new data set has reduced dimensionality.

### Step 6: About test set, do normalization and projection.
-	 Likewise, in step 5, all test data is projected to remaining feature vectors, since each initial data is not valid on new space.

### Step 7: Find the what new training vector is the smallest distance with each new test vector.
-	 The distance between the projected test data and each training data indicates how much the test data is similar to the training data. It can be simply calculated with Euclidean method.

# Result
- When **10 eigen vectors** used, matching **accuracy is 100%**
![eigen10](/assets/eigen10.JPG)
<br>

- When only **one eigen vector** used, matching **accuracy is 75%**
![eigen1](/assets/eigen1.JPG)
<br>

- Graph
![result_graph](/assets/result_graph.png)


# Refence
- PCA / A tutorial on Principal Components Analysis.pdf – Lindsay I Smith / Feb 26, 2002
- Wavelet 압축 영상에서 PCA를 이용한 얼굴 인식률 비교 / 박장한, 남궁 재찬 / 전자공학회논문지 / Sep, 2004
- Principal Component Analysis.pdf / 황선규 / Aug 09, 2005
