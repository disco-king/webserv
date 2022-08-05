import calendar
cl = calendar.HTMLCalendar(firstweekday=0)
str = cl.formatyear(2022)

print(str)