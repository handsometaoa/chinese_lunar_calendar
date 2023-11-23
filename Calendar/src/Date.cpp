#include "Date.h"
#include <stdio.h>
#include <ctime>
#include <string>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <string.h>

using namespace std;


string lunarMonth[]= {" ","正月","二月","三月","四月","五月","六月","七月","八月","九月","十月","冬月","腊月"};
string lunarDay[]= {"零","一","二","三","四","五","六","七","八","九","十"};
string lunarDay2[]= {"初","十","廿","卅"};

/** 农历查询表  来源：https://www.jianshu.com/p/57e7bd43c00b
 *  Ox0c950     是5个16进制数，共20bit
 *  第1位       表示假如有闰月，0代表闰小月，1代表闰大月
 *  第2-4位     表示12个二进制位 一位表示一个月是大月还是小月
 *  第5位       表示闰哪个月，0表示不闰
 */
unsigned int sg_lunarCalendar[199] =
{
    /* 1900年开始，我的日历要求1940-2040年 用不到
    0x04bd8,0x04ae0,0x0a570,0x054d5,0x0d260,0x0d950,0x16554,0x056a0,0x09ad0,0x055d2,
    0x04ae0,0x0a5b6,0x0a4d0,0x0d250,0x1d255,0x0b540,0x0d6a0,0x0ada2,0x095b0,0x14977,
    0x04970,0x0a4b0,0x0b4b5,0x06a50,0x06d40,0x1ab54,0x02b60,0x09570,0x052f2,0x04970,
    0x06566,0x0d4a0,0x0ea50,0x06e95,0x05ad0,0x02b60,0x186e3,0x092e0,0x1c8d7,*/

    0x0c950,/*1939年*/   /*查询公历1940年1月，农历其实是1939年*/
    0x0d4a0,0x1d8a6,0x0b550,0x056a0,0x1a5b4,0x025d0,0x092d0,0x0d2b2,0x0a950,0x0b557,
    0x06ca0,0x0b550,0x15355,0x04da0,0x0a5d0,0x14573,0x052d0,0x0a9a8,0x0e950,0x06aa0,
    0x0aea6,0x0ab50,0x04b60,0x0aae4,0x0a570,0x05260,0x0f263,0x0d950,0x05b57,0x056a0,
    0x096d0,0x04dd5,0x04ad0,0x0a4d0,0x0d4d4,0x0d250,0x0d558,0x0b540,0x0b5a0,0x195a6,
    0x095b0,0x049b0,0x0a974,0x0a4b0,0x0b27a,0x06a50,0x06d40,0x0af46,0x0ab60,0x09570,
    0x04af5,0x04970,0x064b0,0x074a3,0x0ea50,0x06b58,0x055c0,0x0ab60,0x096d5,0x092e0,
    0x0c960,0x0d954,0x0d4a0,0x0da50,0x07552,0x056a0,0x0abb7,0x025d0,0x092d0,0x0cab5,
    0x0a950,0x0b4a0,0x0baa4,0x0ad50,0x055d9,0x04ba0,0x0a5b0,0x15176,0x052b0,0x0a930,
    0x07954,0x06aa0,0x0ad50,0x05b52,0x04b60,0x0a6e6,0x0a4e0,0x0d260,0x0ea65,0x0d530,
    0x05aa0,0x076a3,0x096d0,0x04bd7,0x04ad0,0x0a4d0,0x1d0b6,0x0d250,0x0d520,0x0dd45,
    0x0b5a0,0x056d0,0x055b2,0x049b0,0x0a577,0x0a4b0,0x0aa50,0x1b255,0x06d20,0x0ada0
};

/**
 * 显示欢迎界面
 */
void print()
{
    cout<<"\n\t\t欢迎来到我的日历表（1940-2040年）"<<endl;
    cout<<"\t\t功能选择："<<endl;
    cout<<"\t\t1.查询年日历表"<<endl;
    cout<<"\t\t2.查询月日历表"<<endl;
    cout<<"\t\t3.查询距今多少天？星期几？"<<endl;
    cout<<"\t\t4.输入公历查询农历"<<endl;
    cout<<"\t\t5.输入节气查询对应公历、农历"<<endl;
    cout<<"\t\t*****按0退出*****"<<endl;
}



/** 通式寿星公式 计算二十四节气  公式来源：https://www.jianshu.com/p/1f814c6bb475*/
// 21世纪C值
float C_value_21[]= {3.87,18.73,5.63,20.646,4.81,20.1,5.52,21.04,5.678,21.37,7.108,22.83,7.5,23.13,7.646,23.042,8.318,23.438,7.438,22.36,7.18,21.94,5.4055,20.12};
// 20世纪C值
float C_value_20[]= {4.6295,19.4599,6.3826,21.4155,5.59,20.888,6.318,21.86,6.5,22.2,7.928,23.65,28.35,23.95,8.44,23.822,9.098,24.218,8.218,23.08,7.9,22.6,6.11,20.84};
// 节气基础月
float C_month[]= {2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13};

/**
 * 通过年份节气，获取对应具体日期
 */
void get_date_by_solar_term(int year1,char* str)
{
    int year,month,day;
    year=year1;
    int tmp=-1;//表示节气顺序
    const char* temp[24]= {"立春","雨水","惊蛰","春分","清明","谷雨","立夏","小满","芒种","夏至","小暑","大暑","立秋","处暑","白露","秋分","寒露","霜降","立冬","小雪","大雪","冬至","小寒","大寒"};
    /*比较字符并赋值tmp 如：0就代表立春*/
    for(int i=0; i<24; i++)
    {
        if(strcmp(str,temp[i])==0)
        {
            tmp=i;
            break;
        }
    }
    if(tmp==-1)
    {
        cout<<"节气拼写出错！"<<endl;
        return;
    }

    if(tmp==0||tmp==1||tmp==22||tmp==23)
    {
        if(year>=2000)
        {
            month=C_month[tmp];
            day=floor((year%100)*0.2422+C_value_21[tmp])-floor((year%100-1)/4);
        }
        else
        {
            month=C_month[tmp];
            day=floor((year%100)*0.2422+C_value_20[tmp])-floor((year%100-1)/4);
        }
    }
    else
    {
        if(year>=2000)
        {
            month=C_month[tmp];
            day=floor((year%100)*0.2422+C_value_21[tmp])-floor((year%100)/4);
        }
        else
        {
            month=C_month[tmp];
            day=floor((year%100)*0.2422+C_value_20[tmp])-floor((year%100)/4);
        }
    }

    // 特殊情况
    if((year==2026 && tmp==1)||(year==1918 && tmp==21)||(year==2021 && tmp==21)||(year==2019 && tmp==22))
    {
        day--;
    }
    if((year==2084 && tmp==3)||(year==1911 && tmp==6)||(year==2008 && tmp==7)||(year==1902 && tmp==8)||(year==1928 && tmp==9)||(year==1925 && tmp==10)||(year==2016 && tmp==10)||(year==1922&& tmp==11)||(year==2002&&tmp==12)||(year==1927&&tmp==14)||(year==1942&&tmp==15)||(year==2089 && tmp==17)||(year==2089&&tmp==18)||(year==1978&&tmp==19)||(year==2054 && tmp==20)||(year==1982&&tmp==22)||(year==2000 && tmp==23)||(year==2082&&tmp==23))
    {
        day++;
    }
    if(C_month[tmp]>12)
    {
        month=1;
    }
    cout<<temp[tmp]<<": ";
    // 根据公式计算的年月日，将其转换为农历
    gregorian_to_lunar (year,month,day);
}


/**
 * 获取系统时间
 */
void get_cur_date(int &year,int &month,int &day)
{
    time_t time1;
    struct tm *ptime;
    time(&time1);
    ptime = localtime(&time1);
    year=ptime->tm_year + 1900;
    month=ptime->tm_mon + 1;
    day=ptime->tm_mday;
}

/**
 * 判断year年是否是闰年
 */
bool is_leap_year(int year)
{
    // 可以被4且100整除或者可以被400整除
    return ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0);
}

/**
 * 获取year年month月有多少天
 */
int get_days_of_month(int year,int month)
{
    // 平年各月公历天数
    int days[12]= {31,28,31,30,31,30,31,31,30,31,30,31};
    if(is_leap_year(year)&&month==2)
        return 29;
    else
        return days[month-1];
}

/**
 * 获取year年month月day日到1939年1月1日的天数
 */
int get_days_to_1939(int year,int month,int day)
{
    int days=0;
    // 从1939年开始计算,直到year-1年,它们都是整数年
    for(int i=year-1; i>=1939; i--)
    {
        if(is_leap_year(i))
            days+=366;
        else
            days+=365;
    }
    // 计算year年1月到year年month-1月,它们是整数月
    for(int i=month-1; i>0; i--)
    {
        days+=get_days_of_month(year,i);
    }
    // 计算year年month月1日到yera年month月day日天数
    days+=day;
    return days-1;
}

/**
 * 获取year年month月day日是星期几
 */
string get_str_day_of_week(int year,int month,int day)
{
    string weeks[7]= {"星期日","星期一","星期二","星期三","星期四","星期五","星期六"};
    // 获取year年month月day日到1939年1月1日的天数
    int days=get_days_to_1939(year,month,day);
    int week=days%7;
    return weeks[week];
}
/**
 * 获取year年month月day日是星期几，整数版
 */
int get_day_of_week(int year,int month,int day)
{
    int days=get_days_to_1939(year,month,day);
    int week=days-(days/7)*7;
    return week;
}

/**
 * 获取year年month月day日到现在的天数，正值为过去，负值为未来
 */
int get_days_from_date_to_now(int year,int month,int day)
{
    int nowYear;
    int nowMonth;
    int nowDay;
    get_cur_date(nowYear,nowMonth,nowDay);
    // 现在时间到1939年1月1日的天数
    int nowto1939=get_days_to_1939(nowYear,nowMonth,nowDay);
    // 输入的时间到1939年1月1日的天数
    int tempto1939=get_days_to_1939(year,month,day);
    // 正数为过去，负数为未来
    return nowto1939-tempto1939;
}

/**
 * 检查年月日合法性
 */
bool is_legal(int year,int month,int day)
{
    // 还需要判断 year年month月  是否含day天，比如4月有30天，输入31就是错误的
    return !(year<1939||year>2041||month<0||month>12||day<0||day>get_days_of_month(year,month));
}








/**
 * 判断润年闰月是大月还是小月
 */
bool is_large_month_of_leap_year(int year)
{
    // 查表找出是否为闰月 判断high是不是为0
    int high =(sg_lunarCalendar[year-1939]>>16);
    if(high==0x00)
        return false;
    else
        return true;
}
/**
 * 返回 润几月  0代表不闰
 */
int get_nonth_of_leap_year(int year)
{
    // 输出农历润几月
    int month = sg_lunarCalendar[year-1939] & 0xF;
    return month;
}

/**
 * 判断农历某年某月多少天
 */
int get_days_of_lunar_month(int year,int month)
{
    // 查表，根据大小月，计算农历月天数
    int bit=1 << (16-month);
    if((sg_lunarCalendar[year-1939]/*&0x0000FFFF*/&bit)==0)
        return 29;
    else
        return 30;
}

// 判断某年农历多少天
int get_days_of_lunar_year(int year)
{
    int days=0;
    // 农历十二个月天数
    for(int i=1; i<=12; i++)
    {
        days+=get_days_of_lunar_month(year,i);
    }
    // 假如有闰月，加上闰月天数
    if(get_nonth_of_leap_year(year)!=0)
    {
        if(is_large_month_of_leap_year(year))
            days+=30;
        else
            days+=29;
    }
    return days;
}


/**
 * 显示距今多少天
 */
void print_number_of_date_to_now(int year,int month,int day)
{
    if(!is_legal(year,month,day))
    {
        cout<<"日期输入出错，请确保日期正确性！"<<endl;
        return;
    }

    int days=get_days_from_date_to_now(year,month,day);
    if(days>0)
        cout<<year<<"年"<<month<<"月"<<day<<"日\t"<<get_str_day_of_week(year,month,day)<<"\t距现在已过去"<<days<<"天"<<endl;
    else
        cout<<year<<"年"<<month<<"月"<<day<<"日\t"<<get_str_day_of_week(year,month,day)<<"\t距现在还有"<<-days<<"天"<<endl;
}


/**
 * 输入年份月份输出月日历表
 */
void print_monthly_calendar(int year,int month)
{
    if(year>2041||year<1939||month>12||month<1)
    {
        cout<<"日期输入出错，请确保日期在1939-2041年之间"<<endl;
        return;
    }
    cout<<"\t"<<year<<"年"<<month<<"月"<<endl;
    cout<<" SUN MON TUE WED THU FRI SAT"<<endl;
    // 计算year年month月1日星期几
    int week=get_day_of_week(year,month,1);
    // 调整1日到对应的星期数
    for(int i=1; i<=week&&week!=7; i++)
        cout<<"    ";
    // 打印日历信息
    for(int i=1; i<=get_days_of_month(year,month); i++)
    {
        cout<<setw(3)<<i<<" ";
        if((week+i)%7==0)
            cout<<endl;
    }
}

/**
 * 输入年份、季度 一行输出3个月
 */
void print_seasons_calendar(int year,int num)
{
    if(num>4||num<0)
    {
        cout<<"输入季度出错！"<<endl;
        return;
    }
    int month=1+3*(num-1);
    // 打印日历头
    cout<<month<<" SUN MON TUE WED THU FRI SAT\t"<<month+1<<" SUN MON TUE WED THU FRI SAT\t"<<month+2<<" SUN MON TUE WED THU FRI SAT"<<endl;
    // week1、week2、week3 分别代表一个季度的三个月的1日对应星期数
    int week1=get_day_of_week(year,month,1);
    int week2=get_day_of_week(year,month+1,1);
    int week3=get_day_of_week(year,month+2,1);

    // tmp1、tmp2、tmp3分别代表三个月的最大天数
    int tmp1=get_days_of_month(year,month);
    int tmp2=get_days_of_month(year,month+1);
    int tmp3=get_days_of_month(year,month+2);

    int day1,day2,day3;

    /*依次输出第一行信息*/
    cout<<"  ";
    for(int i=1; i<=week1 &&week1!=7; i++)
        cout<<"    ";
    for(int i=1; i<=tmp1; i++)
    {
        cout<<setw(3)<<i<<" ";
        if((week1+i)%7==0)
        {
            day1=i+1;
            break;
        }
    }
    cout<<"    ";
    for(int i=1; i<=week2 && week2!=7; i++)
        cout<<"    ";
    for(int i=1; i<=tmp2; i++)
    {
        cout<<setw(3)<<i<<" ";
        if((week2+i)%7==0)
        {
            day2=i+1;
            break;
        }
    }
    cout<<"    ";
    for(int i=1; i<=week3 && week3!=7; i++)
        cout<<"    ";
    for(int i=1; i<=tmp3; i++)
    {
        cout<<setw(3)<<i<<" ";
        if((week3+i)%7==0)
        {
            day3=i+1;
            cout<<endl;
            break;
        }
    }

    // 打印余下信息
    for(int j=0; j<5; j++)
    {
        // 如果三个月信息全部打印完，则退出
        if(day1>tmp1&&day2>tmp2&&day3>tmp3)
            break;
        // 数字21代表一行三个周，可容纳21天
        for(int i=0; i<21;)
        {
            cout<<"  ";
            while(i<7)
            {
                i++;
                // 如果没打印完就打印数字，如果整月天数已经打印完，则输出空格
                if(day1<=tmp1)
                    cout<<setw(3)<<day1++<<" ";
                else
                    cout<<setw(3)<<" "<<" ";
            }
            // 打印月份与月份之间的间隔
            cout<<"    ";
            while(i<14&&i>=7)
            {
                i++;
                if(day2<=tmp2)
                    cout<<setw(3)<<day2++<<" ";
                else
                    cout<<setw(3)<<" "<<" ";
            }
            cout<<"    ";
            while(i<21&&i>=14)
            {
                i++;
                if(day3<=tmp3)
                    cout<<setw(3)<<day3++<<" ";
                else
                    cout<<setw(3)<<" "<<" ";
            }
        }
        cout<<endl;
    }
    cout<<endl;
}



/**
 * 输出年日历表
 */
void print_annual_calendar(int year)
{
    if(year>2041||year<1939)
    {
        cout<<"日期输入出错，请确保日期正确性！"<<endl;
        return;
    }
    cout<<"\t\t\t\t\t  "<<"------------"<<endl;
    cout<<"\t\t\t\t\t  "<<year<<"年日历表"<<endl;
    cout<<"\t\t\t\t\t  "<<"------------"<<endl;

    // 循环输出四个季度 打印完整年日历
    for(int i=1; i<=4; i++)
    {
        print_seasons_calendar(year,i);
    }
}


/**
 * 输出农历日历
 */
void print_lunar_date(int days)
{
    int year=1939,month=1,day=1;
    // 由于1939年1月1日往往是农历1938年，所以减去48天，从1939年正月初一计算，因为本计算器计算范围为1940年-2040年 所以可以满足
    days-=48;

    // 判断减去下一年的天数，会不会是负值，不是便减去并增加农历年
    for(int i=1939; (days-get_days_of_lunar_year(i))>0; i++)
    {
        days-=get_days_of_lunar_year(i);
        year=i+1;
    }
    int num;
    int signal=0;
    // 判断减去下一月天数，会不会是负值，不会便农历月加1
    for(int j=1; ((days-get_days_of_lunar_month(year,j))>0)&&j<12; j++)
    {
        days-=get_days_of_lunar_month(year,j);
        if(j==get_nonth_of_leap_year(year))
        {
            if(is_large_month_of_leap_year(year))
            {
                days-=30;
                num=30;
            }
            else
            {
                days-=29;
                num=29;
            }
        }
        // 当减去农历月的时候，加入变为负值，那么再加回来
        if(days<0)
        {
            // 农历闰月标记
            signal=1;
            days +=num;
            month=j;
            break;
        }
        month=month+1;
    }
    // 剩余天数便是农历日
    day=days;
    cout<<year<<"年";
    if(signal==1)
        cout<<" 闰";
    cout<<lunarMonth[month];
    if(day>0&&day<=10)
    {
        cout<<"初"<<lunarDay[day];
    }
    else if(day>10&&day<20)
    {
        cout<<"十"<<lunarDay[day%10];
    }
    else if(day==20)
    {
        cout<<"二十";
    }
    else if(day>20 && day<30)
    {
        cout<<"廿"<<lunarDay[day%10];
    }
    else if(day==30)
    {
        cout<<"三十";
    }
    cout<<endl;
}

/**
 * 打印农历
 */
void gregorian_to_lunar (int year,int month,int day)
{
    if(!is_legal(year,month,day))
    {
        cout<<"日期输入出错，请确保日期正确性！"<<endl;
        return;
    }
    cout<<"公历:"<<year<<"年"<<month<<"月"<<day<<"日"<<"   "<<"农历:";
    int days=get_days_to_1939(year,month,day);
    // 根据天数打印农历
    print_lunar_date(days);
}


