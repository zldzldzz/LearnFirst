# 사람은 태어난 해에 맞춰 쥐띠·소띠·범띠·토끼띠·용띠·뱀띠·말띠·양띠·잔나비띠·닭띠·개띠·돼지띠 등 12가지 동물(띠) 
# 중 하나를 부여받는데 4장의 카드에 동물을 배치하고 상대방에게 이 카드를 보여준 후 각 카드에 대해 있는지 없는지에 대한 답변으로 상대방의 띠를 알 수 있는 프로그램을
import random
Year=["쥐띠","소띠","범띠","토끼띠","용띠","뱀띠","말띠","양띠","잔나비띠","닭띠","개띠","돼지띠"]
correct=random.choice(Year)
answer=[]
noAnswer=[]
yesAnswer=[]

while(len(yesAnswer)!=4):
    answer=random.sample(Year, 4)
    if correct in answer:
        print(*answer,"중에 있습니다.")
        for a in answer:
            Year.remove(a)
        for _ in Year:
            noAnswer.append(_)        
        yesAnswer=answer
    else:
        print(*answer,"중에 없습니다.")
        for _ in answer:
            noAnswer.append(_)
            Year.remove(_)

answer=random.sample(yesAnswer, 2)
answer.extend(random.sample(noAnswer,2))
if correct in answer:
    print(*answer,"중에 있습니다.")
    yesAnswer = [item for item in yesAnswer if item in answer]
    noAnswer.append(answer.pop())  # 그 다음 값을 꺼내 noAnswer에 추가
    noAnswer.append(answer.pop())  # 그 다음 값을 꺼내 noAnswer에 추가

else:
    print(*answer,"중에 없습니다.")
    for i in range(2):  # i를 사용하여 인덱스 접근
        yesAnswer.remove(answer[i])
        noAnswer.append(answer[i])


answer=random.sample(yesAnswer, 1)
answer.extend(random.sample(noAnswer,3))
if correct in answer:
    print(*answer,"중에 있습니다")
    print(answer[0])
else:
   print(*answer,"중에 없습니다")
   yesAnswer.remove(answer[0])
   print("정답 :",yesAnswer[0])