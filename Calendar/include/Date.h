#ifndef DATE_H
#define DATE_H

/**
 * ������ݡ��·ݡ����ڣ�����õ�������һ��ݽ����ж�����?���ڼ�?
 */
void print_number_of_date_to_now(int year,int month,int day);

/**
 * �������£�������µ�������
 */
void print_monthly_calendar(int year,int month);

/**
 * ��������һ����ݣ����������Ļ����ʾ�����������*/
void print_annual_calendar(int year);

/**
 * ���빫������ʾũ����
 */
void gregorian_to_lunar (int year,int month,int day);

/**
 * ������ݣ���ʮ������
 */

void get_date_by_solar_term(int year,char* str);
/**
 * ��ӭ����
 */
void print();

#endif // DATE_H
