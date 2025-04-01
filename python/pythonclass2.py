# 사람은 태어난 해에 맞춰 쥐띠·소띠·범띠·토끼띠·용띠·뱀띠·말띠·양띠·잔나비띠·닭띠·개띠·돼지띠 등 12가지 동물(띠) 
# 중 하나를 부여받는데 4장의 카드에 동물을 배치하고 상대방에게 이 카드를 보여준 후 각 카드에 대해 있는지 없는지에 대한 답변으로 상대방의 띠를 알 수 있는 프로그램을
import random
Year=["쥐띠","소띠","범띠","토끼띠","용띠","뱀띠","말띠","양띠","잔나비띠","닭띠","개띠","돼지띠"]
correct=random.choice(Year)
answer=[]
noAnswer=[]
yesAnswer=[]
countR=0
print(correct)
while (countR!=1):
    answer= list(map(str, input().split()))
    if (correct in answer):
        if (len(answer)==1):
            print("정답")
            countR=1
        else:
            print("포함되어 있습니다.")
    elif(len(answer)>4):
        print("4이하의 갯수를 물어보세요.")
         
    else:
            print("포함되어있지 않습니다.")