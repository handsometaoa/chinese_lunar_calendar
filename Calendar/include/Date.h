#ifndef DATE_H
#define DATE_H

/*输入年份、月份、日期，计算得到的是这一天据今天有多少天?星期几?*/
void print_number_week(int year,int month,int day);
/*输入年月，输出该月的日历。*/
void print_monthly_calendar(int year,int month);
/*输入输入一个年份，输出是在屏幕上显示该年的日历。*/
void print_annual_calendar(int year);
//输入公历，显示农历。
void gregorian_to_lunar (int year,int month,int day);
//输入年份，二十四气节
void solar_term(int year,char* str);
//欢迎界面
void print();
#endif // DATE_H
