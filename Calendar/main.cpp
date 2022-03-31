#include <iostream>
#include "Date.h"
using namespace std;

int main()
{

    int num;
    char solar_Terms[3];
    int year,month,day;
    print();
    while(1)
    {

        cout<<"\n请选择服务序号：";
        cin>>num;
        switch(num)
        {
        case 1:
            cout<<"请输入要查询的年份：";
            cin>>year;
            print_annual_calendar(year);
            break;
        case 2:
            cout<<"请输入要查询的年、月：";
            cin>>year>>month;
            print_monthly_calendar(year,month);
            break;
        case 3:
            cout<<"请输入要查询的年、月、日：";
            cin>>year>>month>>day;
            print_number_week(year,month,day);
            break;
        case 4:
            cout<<"请输入要查询的年、月、日：";
            cin>>year>>month>>day;
            gregorian_to_lunar (year,month,day);
            break;
        case 5:
            cout<<"请输入要查询的年份及节气：";
            cin>>year>>solar_Terms;
            solar_term(year,solar_Terms);
            break;
        case 0:
            return 0;
        }
    }
    return 0;
}
