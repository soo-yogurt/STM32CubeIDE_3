#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


void Init_data_list(size_t id_list_len, int data[][id_list_len], int sum[])
{
    for (int i = 0; i < id_list_len; i++)
    {
        for (int j = 0; j < id_list_len; j++)
        {
            data[i][j] = 0;
        }
        sum[i] = 0;
    }
}


// id_list_len�� �迭 id_list�� �����Դϴ�.
// report_len�� �迭 report�� �����Դϴ�.
// �Ķ���ͷ� �־����� ���ڿ��� const�� �־����ϴ�. �����Ϸ��� ���ڿ��� �����ؼ� ����ϼ���.
int* solution(const char* id_list[], size_t id_list_len, const char* report[], size_t report_len, int k) {
    // return ���� malloc �� ���� �Ҵ��� ������ּ���. �Ҵ� ���̴� ��Ȳ�� �°� �������ּ���.
    // ó�� ����� ���� Ƚ���� return �Ѵ�.

    size_t maximum_length_of_report_element = 21;
    int data[id_list_len][id_list_len];
    int sum[id_list_len];


    Init_data_list(id_list_len, data, sum);

    char temp[report_len][maximum_length_of_report_element];
    memset(temp, 0, sizeof(temp));

    for (int i = 0; i < report_len; i++) {
        strcpy(temp[i], report[i]);
    }




    for (int i = 0; i < report_len; i++)
    {
        char* new_report = (char*)malloc(strlen(report[i]) + 1);
        strcpy(new_report, report[i]);

        char* p_tok;
        p_tok = strtok(new_report, " ");

        int i;
        for (i = 0; i < id_list_len; i++) {
            if (p_tok == id_list[i]) {
                break;
            }
        }

        p_tok = strtok(NULL, " ");

        int j;
        for (j = 0; j < id_list_len; j++)
        {
            if (p_tok == id_list[j]) {
                break;
            }
        }
        data[i][j] += 1;

        free(new_report);
    }

    /*
    char* p;

    p = strtok(report)




    for(int i = 0; i < id_list_len; i++)
    {
        for(int j = 0; j < id_list_len; j++)
        sum[i] += data[j][i];
    }

    for(int i = 0; i < id_list_len; i++)
    {
        if(sum[i] >= k)


    }
    */


    int* answer = (int*)malloc(sizeof(int) * id_list_len);


    return answer;
}