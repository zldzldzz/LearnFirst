# h=input("학과:")
# x=input("이름:")
# y=int(input("학년:"))
# print("안양대학교",h, y,"학년",x,"입니다.")
# x="제이름은"
# y=input()
# x=x+y+"입니다."
# print(x)

# even=[2,4,6,8]
# add=[1,3,5,7]
# all=add+even
# print(all*2)

# name=input("이름:")
# year=2024
# age=int(input("출생년:"))

# print("나는 %s이고 나이는 %s입니다." %(name, year-age))

# astring='hello world!'
# print(astring.index("o"))
# print(astring.count("l"))
# print(astring[3:7])
# print(astring[3:7:2])

# print(astring[::-1])
# print(astring.upper())
# print(astring.startswith('hello'))
# a=astring.split(" ")
# print(a)

# print(set("my name is Eric and Eric is my name".split()))
# aset=set(['apple','banana'])
# bset=set(['orinde','apple'])
# cset=aset.union(bset)
# print(cset)
# dset=bset.union(aset)
# print(dset)

# x=2
# print(x==2)
# print(x==3)
# print(x!=3)
# print(x<3)

# name='wefim'
# age=23
# if name =='wefim' and age==23:
#     print("성공")
# if name =='wefim' or name=='ewfjionrepin':
#     print('성공2')

# x=input("값을 입력하세요: ")
# x=2
# y=2
# print(x is y)
# print(not False)

# if x==2:
#     print("x는 2이다")
# else:
#     print("x는 2가 아니다")
# if -1:
#     # 0은 거짓취급
#     print("true")


#다음에도 쓸 수 있다.
# time=int(input('what time? : '))
# m_time = int(input('몇분? : '))
# time = time*100+m_time
# class_start = 1600
# if class_start - time < 10:
#     print('지각')
# elif class_start < time:
#     print('지각')
# else:
#     money = int(input('얼마냐? : '))
#     if money >= 6000 :
#         print('taxi')
#     else :
#         print('뛰어라')

#무조건 1번 이상 실행
# prmes=[2,3,5,7]
# for prmes in prmes:
#     print(prmes)
# print("\n")
# for prmes in range (5):
#     print(prmes)
# print("\n")
# for prmes in range (3,6):
#     print(prmes)
# print("\n")
# for prmes in range (3,8,2):
#     print(prmes)

# count=0
# while count <5:
#     print(count)
#     count+=1
# print("\n")

# count=0
# while count <5:
#     print(count)
#     count=count+1

# sum=0
# y=0
# x=int(input())
# while(y<=x):
#     print(sum)
#     sum+=y
#     y+=1
# print(sum)

# count =0
# while count <5:
#     print(count)
#     count+=1
# else:
#     print("깂%d" %(count))
# print("\n")

# count =0
# for i in range (1,10):
#     if(i%5==0):
#         break
#     else: 
#         print(i)

# def fun():
#     print("안녕 함수야")
# def sum(a,b):
#     print(a+b)
# fun()
# sum("안녕","이것도")

# def nsum():
#     sum=0
#     y=0
#     x=int(input("n 입력 해라:"))
#     while(y<=x):
#         sum+=y
#         y+=1
#     print(sum)
# nsum()

# class MyClass:
#     variable="blah"
#     def function (self):
#         print("이거는ㄴ 클ㄹ래스 안에 있어")

# myobjectx=MyClass()
# myobjecty=MyClass()

# myobjecty.variable="yackity"

# print(myobjectx.variable)
# print(myobjecty.variable)

# myobjectx.function()

# class Student:
#     "this is a studet class"
#     age =21
#     def greet(self):
#         print("hello")
# jaime=Student()
# print(Student.greet)
# print(jaime.greet())
# print(jaime)

# jaime.greet()#이런식으로 써야함

# class Complex:
#     def __init__(self,r=0,i=0):
#         self.real=r
#         self.imag=i

#     def get_data(self):
#         print(f'{self.real},{self.imag}')

# num2= Complex(5)
# num1=Complex()
# num2.attr=10
# num1.attr=10

# print((num2.real,num2.imag,num2.attr))
# print(num1.attr)
# num1=Complex(3,-4.5)

# print("\n")
# num1.get_data()

# phonebook={
#     "lee":12332414,
#     "jeang":123423,
#     "han":23413545
# }
# print(phonebook)
# phonebook={}
# phonebook["jeang"]=132
# phonebook["lee"]=312
# print(phonebook)

# for name, number in phonebook.items():
#     print("%s의 번호는 %d입니다"%(name,number))

# def myfun1():# 첫번째 함수 선언
#     x='lee'#지역변수 x에 입력
#     def myfun2():# 두번째 함수 선언
#         nonlocal x #논지역변수 x 재선언
#         x="hello" #별도로 리턴하지 않음
#     myfun2()
#     return x# 함수 1의 결과가 리턴 받지 않아도 2번의 결과에 따라 결정
# print(myfun1())

# def myfun1():
#     x='lee'
#     def myfun2():
#         x="hello"
#     myfun2()
#     return x
# print(myfun1())

# try:
#     print(x)#x의 값이 있으면 else 
#     #무조건 finally실행
#     #값이 없는경우 except실행
# except:
#     print("qwef")
# else:
#     print("wef")
# finally:
#     print("wef")

# # 파일 쓰기
# f = open("test.txt", "w")
# f.write("hellowqeFGVAfqw")
# f.close()  # 파일을 반드시 닫아줍니다.

# # 파일 읽기
# f = open("test.txt", "r")
# content = f.read()  # 파일 내용을 읽습니다.
# print(content)  # 읽은 내용을 출력합니다.
# f.close()  # 파일을 닫아줍니다.

# from tkinter import *
# from tkinter.filedialog import askopenfilename
# from tkinter.filedialog import asksaveasfilename
# fd=askopenfilename()
# if(fd!=None):
#     f3=open(fd,"r")
#     fr=f3.readline()
# for line in fr:
#     line= line.rstrip()
#     print(line,end='')

#함수를 이용해서 n!를 구하는 프로그램을 작성하시오

# class nP:
#     def __init__(self):
#         self.result = 1

#     def getmul(self,x, num):

#         self.result = x * num 
#         return self.result

#     def nFor(self, x, t):#3
#         for n in range(1, t + 1): 
#             x = self.getmul(x,n) 
#         return x

# nP1 = nP()
# t = int(input(" t입력해라: "))  
# x = 1
# x = nP1.nFor(x, t) 

# print(x) 


# pone={"lee":123,
#       "toy":1234,
#       "hun":4444
#       }
# for name, num in pone.items():
#     print(name,num)

from tkinter import *
from tkinter.filedialog import askopenfilename
fd=askopenfilename()
con=0
if fd:
    f3=open(fd,"r")
    fr=f3.read()
    li=fr.split()
    print(len(li))
    inp=input("입력하라유")
    for x in li:
        if x==inp:
            con+=1
    f3.close
print(con)