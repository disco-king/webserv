import calendar
import locale; locale.setlocale(category=locale.LC_ALL,locale="en_US.utf8")
cal=calendar.LocaleHTMLCalendar(firstweekday=0, locale='en_US.utf8')
# декодируем HTML-строку 
str=cal.formatyearpage(2019,3, css="calendar.css").decode('utf8')
# публикуем в файл
with open('calendar.html', 'w', encoding='utf8') as f:
     print(str, file=f)
# a = calendar.LocaleHTMLCalendar(locale='Russian_Russia')
# # with open('calendar.html', 'w') as g:
# #     print(a.formatyear(2014, width=4), file=g)