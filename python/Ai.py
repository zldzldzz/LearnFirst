import pandas as pd #data anaylsis
import matplotlib.pyplot as plt #visualization
from sklearn import datasets
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split
from sklearn.neighbors import KNeighborsClassifier
from sklearn import metrics


iris = datasets.load_iris()


# data=pd.DataFrame(iris.data)
# print(data)
# plt.scatter(data.iloc[:,[0]],data.iloc[:,[1]],c=iris.target)
# plt.show()

X = iris.data #꽃 문제
y = iris.target #꽃 문제

# (80:20)으로 분할한다.
X_train,X_test,y_train,y_test = train_test_split(X,y,test_size=0.2,random_state=4)


knn = KNeighborsClassifier(n_neighbors=6)
knn.fit(X_train, y_train)

y_pred = knn.predict(X_test)

scores = metrics.accuracy_score(y_test, y_pred)
print(scores)

