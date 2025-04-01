import matplotlib.pyplot as plt

k=1
time={}
week=["M", "Tu", "W", "Th", "F",  "Sa", "Su" ]

while k != 1:
    dayOfTheWeek=input("요일 입력해라:")
    if dayOfTheWeek in week:
        print(dayOfTheWeek,"시간 입력해라:")
        dayOfTheWeekTime=int(input())
        time[dayOfTheWeek]=dayOfTheWeekTime
    else:
        print(*week,"중에 입력해라 쓰발놈아")

    if dayOfTheWeek=="끝":
        break
plt.plot(time.keys(),time.values())
plt.show()