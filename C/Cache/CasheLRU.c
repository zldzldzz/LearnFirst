#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable : 4996)
/* ���� �̸� */
#define filename "test.txt"

struct i_cache
{ /* �ν�Ʈ���� ĳ�� ����ü */
    int tag;
    int valid;
    int time;
};

struct d_cache
{              /* ������ ĳ�� ����ü */
    int tag;   /* �±� */
    int valid; /* valid bit */
    int time;  /* LRU�� �����ϱ� ���� ������ ���� �ð�*/
    int dirty; /* write back �� dirty bit */
};

/* ���� ���� */
int i_total, i_miss;          /* �ν�Ʈ���� ĳ�� ���� Ƚ�� �� miss Ƚ�� */
int d_total, d_miss, d_write; /* ������ ĳ�� ���� Ƚ�� �� miss Ƚ��, �޸� ���� Ƚ�� */
int time_count;               /* LRU�� �����ϱ� ���� �ð� */
int s_miss, l_miss;
int s_hit, l_hit;

struct i_cache *ip; /* �ν�Ʈ���� ĳ�ø� ����Ű�� ������ */
struct d_cache *dp; /* ������ ĳ�ø� ����Ű�� ������ */

/* �Լ� ��� */
void simulation(int c_size, int b_size, int assoc);
void read_data(int addr, int c_size, int b_size, int assoc);
void write_data(int addr, int c_size, int b_size, int assoc);
void fetch_inst(int addr, int c_size, int b_size, int assoc);
int evict(int set, int assoc, char mode);

int main()
{
    int cache[5] = {1024, 2048, 4096, 8192, 16384}; /* ĳ�� ������ */
    int block[2] = {16, 64};                        /* �� ������ */
    int associative[4] = {1, 2, 4, 8};              /* associative set�� ���ΰ��� */
    int i, j, k;                                    /* �ݺ��� �ε��� */
    int input = 0;

    while (input != 3)
    {
        printf("1. print all\n2. select\n3. exit\n: ");
        scanf("%d", &input);
        // input�� 1�� ��� �ùķ��̼��� �� ��Ȳ�� ��� �����ϱ�
        // input�� 2�� ��� �ùķ��̼��� ���� ��Ȳ�� ����(cache size,block size ,associative) 3���� ������ Ű����� �޾Ƽ� ����
        // input�� 2�� ��� �ùķ��̼��� �������� �ʰ� ����

        /* ��ü �ùķ��̼� */
        if (input == 1)
        {
            printf("cache size | block size | associative | d-miss rate | i-miss rate | mem write\n");
            for (i = 0; i < 2; i++)
            {
                for (j = 0; j < 5; j++)
                {
                    for (k = 0; k < 4; k++)
                    {
                        simulation(cache[j], block[i], associative[k]);
                    }
                }
            }
        }
        /* ���� �ùķ��̼� */
        else if (input == 2)
        {
            printf("cache size | block size | associative | d-miss rate | i-miss rate | mem write\n");
            printf("cache size : ");
            scanf("%d", &i);
            printf("block size : ");
            scanf("%d", &j);
            printf("associative : ");
            scanf("%d", &k);

            simulation(i, j, k);
        }
    }
    return 0;
}

/* �ùķ��̼� �Լ�: ĳ�û������ ��������, associative�� ���ڷ� �޴´�. */
void simulation(int c_size, int b_size, int assoc)
{
    /* �������� �� �ʱ�ȭ */
    i_total = i_miss = 0;
    d_total = d_miss = d_write = 0;
    s_miss = l_miss = 0;
    s_hit = l_hit = 0;

    char mode;                 // ��ɾ��� �ǹ� �б� s�� �б� �� ���� ����, l�� �б� ����, f�� ��� �ν�Ʈ���� ����
    int addr;                  /* ���Ϸκ��� ���� ���� �����ϴ� ���� */
    int num = c_size / b_size; // �� ĳ���� ũ��
    double i_res, d_res;       /* miss rate�� �����ϴ� ���� */
    FILE *fp = NULL;

    ip = (struct i_cache *)calloc(num, sizeof(struct i_cache)); //
    dp = (struct d_cache *)calloc(num, sizeof(struct d_cache)); //

    fp = fopen(filename, "r"); // �б� ���Ϸ� ���� ����
    /* ���Ϸκ��� mode�� �ּҸ� �о� �� �Լ��� �����Ѵ�. */
    while (fscanf(fp, " %c %x", &mode, &addr) == 2)
    {
        switch (mode)
        {
        case 108: // 'l'
            read_data(addr, c_size, b_size, assoc);
            d_total++;
            break;
        case 115: // 's'
            write_data(addr, c_size, b_size, assoc);
            d_total++;
            break;
        case 102: // 'f'
            fetch_inst(addr, c_size, b_size, assoc);
            i_total++;
            break;
        }
        time_count++;
    }

    free(ip);
    free(dp);
    i_res = (i_total != 0) ? (double)i_miss / i_total : 0.0; // �ν�Ʈ���Ǻ��� ���ϱ�
    d_res = (d_total != 0) ? (double)d_miss / d_total : 0.0; // ������miss�������ϱ�

    // ��� ���
    printf("ĳ��_size: %5d, ���_size: %5d, set�� ���μ�: %5d\n������miss����: %.4lf, �ν�Ʈ���Ǻ���: %.4lf, ������ ����Ƚ��:  %5d \n", c_size, b_size, assoc, d_res, i_res, d_write);
    printf("l_miss %5d s_miss %5d ��ü_miss: %5d\n", l_miss, s_miss, d_miss);
    printf("l_hit %5d  %5d dh %5d\n\n", l_hit, s_hit, (d_total - d_miss));

    fclose(fp);
}

void read_data(int addr, int c_size, int b_size, int assoc)
{
    int num_of_sets, set; /* set�� ������ �Է¹��� �ּ��� set�� �����ϴ� ���� */
    int i, ev = 0;        /* �ݺ����� �ε����� victim�� �ε���, �׸��� ���� ���� ���� �ε��� */
    int avail = assoc;
    struct d_cache *p; /* ĳ�ø� ����Ű�� ������ */

    num_of_sets = c_size / (b_size * assoc); /* set�� ������ ���Ѵ�. */
    set = (addr / b_size) % num_of_sets;     /* �Է¹��� ���ڷκ��� �ش� �ּ��� set�� ���Ѵ�. */

    /* ĳ�ÿ��� �ش� set�� �˻��Ͽ� HIT/MISS�� ���� */
    for (i = 0; i < assoc; i++)
    {
        p = &dp[set * assoc + i]; // �Է� ���� ��ɾ�� ���� ��ġ�� set�� �˻��ϰ� ã��
        /* valid bit�� 1�̰� tag���� ��ġ�ϸ� ���� �ð��� �ٲٰ� HIT */
        if (p->valid == 1 && p->tag == (addr / b_size) / num_of_sets)
        {
            l_hit++;
            p->time = time_count; // ���� �ð� �߰� ��ü����� ����
            return;
        }
        /* ���ο� ���� �� �ε��� */
        else if (p->valid == 0)
        {
            if (i < avail)
                avail = i;
        }
    }
    /* set�� �ش�Ǵ� ���� �����Ƿ� ������ �о���� MISS�̰� ���ο� ���� �ø���. */
    d_miss++, l_miss++;
    /* ĳ���� set�� ������ ��� */
    if (avail == assoc)
    {
        ev = evict(set, assoc, 'd'); // evict�� ���� ��ü�� ��� ��������
        p = &dp[set * assoc + ev];   // ������ �ִ� ���� ��ü�ؾ� �ϴ� ��ġ�� �̵�

        /* victim ���� dirty bit�� 1�̸� �޸� ���⸦ �Ѵ�. */
        if (p->dirty)
            d_write++;

        p->valid = 1;                           // ��ȿ�� �����Ͷ�� üũ
        p->time = time_count;                   // ���� �� �ð� üũ
        p->tag = (addr / b_size) / num_of_sets; // set��ġ ����
        p->dirty = 0;                           // ���� �ø� ���̹Ƿ� dirty bit�� 0�̴�.
    }
    /* ĳ���� set�� �ڸ��� �ִ� ��� */
    else
    {
        p = &dp[set * assoc + avail];

        p->valid = 1;                           // ��ȿ�� �����Ͷ�� üũ
        p->time = time_count;                   // ���� �� �ð� üũ
        p->tag = (addr / b_size) / num_of_sets; // set��ġ ����
        p->dirty = 0;                           // ���� �ø� ���̹Ƿ� dirty bit�� 0�̴�.
    }
}

void write_data(int addr, int c_size, int b_size, int assoc)
{
    int num_of_sets, set;         /* set�� ������ �Է¹��� �ּ��� set�� �����ϴ� ���� */
    int i, ev = 0, avail = assoc; /* �ݺ����� �ε����� victim�� �ε���, �׸��� ���� ���� ���� �ε��� */
    struct d_cache *p;            /* ĳ�ø� ����Ű�� ������ */

    num_of_sets = c_size / (b_size * assoc); /* set�� ������ ���Ѵ�. */
    set = (addr / b_size) % num_of_sets;     /* �Է¹��� ���ڷκ��� �ش� �ּ��� set�� ���Ѵ�. */

    /* ĳ�ÿ��� �ش� set�� �˻��Ͽ� HIT/MISS�� ���� */
    for (i = 0; i < assoc; i++)
    {
        p = &dp[set * assoc + i];
        /* valid bit�� 1�̰� tag���� ��ġ�ϸ� ���� �ð��� �ٲٰ�, dirty bit�� 1�� �����ϰ� HIT */
        if (p->valid == 1 && p->tag == (addr / b_size) / num_of_sets)
        {
            p->time = time_count;
            s_hit++;
            p->dirty = 1;
            return;
        }
        else if (p->valid == 0)
        {
            if (i < avail)
                avail = i;
        }
    }
    /* set�� �ش�Ǵ� ���� �����Ƿ� MISS�̰� ���ο� ���� �ø���. */
    d_miss++, s_miss++;
    /* ĳ���� set�� ������ ��� */
    if (avail == assoc)
    {
        ev = evict(set, assoc, 'd');
        p = &dp[set * assoc + ev];

        /* victim ���� dirty bit�� 1�̸� �޸� ���⸦ �Ѵ�. */
        if (p->dirty)
            d_write++;

        p->valid = 1;
        p->time = time_count;
        p->tag = (addr / b_size) / num_of_sets;
        p->dirty = 1; /* ���� �ø� ���� ���������Ƿ� dirty bit�� 1 */
    }
    /* ĳ���� set�� �ڸ��� �ִ� ��� */
    else
    {
        p = &dp[set * assoc + avail];
        p->valid = 1;
        p->time = time_count;
        p->tag = (addr / b_size) / num_of_sets;
        p->dirty = 1;
    }
}

void fetch_inst(int addr, int c_size, int b_size, int assoc)
{
    int num_of_sets, set;         /* set�� ������ �Է¹��� �ּ��� set�� �����ϴ� ���� */
    int i, ev = 0, avail = assoc; /* �ݺ����� �ε����� victim�� �ε���, �׸��� ���� ���� ���� �ε��� */
    struct i_cache *p;            /* ĳ�ø� ����Ű�� ������ */

    num_of_sets = c_size / (b_size * assoc); /* set�� ������ ���Ѵ�. */
    set = (addr / b_size) % num_of_sets;     /* �Է¹��� ���ڷκ��� �ش� �ּ��� set�� ���Ѵ�. */

    /* ĳ�ÿ��� �ش� set�� �˻��Ͽ� HIT/MISS�� ���� */
    for (i = 0; i < assoc; i++)
    {
        p = &ip[set * assoc + i];
        /* valid bit�� 1�̰� tag���� ��ġ�ϸ� ���� �ð��� �ٲٰ� HIT */
        if (p->valid == 1 && p->tag == (addr / b_size) / num_of_sets)
        {
            p->time = time_count;
            return;
        }
        else if (p->valid == 0)
        {
            if (i < avail)
                avail = i;
        }
    }

    /* set�� �ش�Ǵ� ���� �����Ƿ� MISS�̰� ���ο� ���� �ø���. */
    i_miss++;

    /* ĳ���� set�� ������ ��� */
    if (avail == assoc)
    {
        ev = evict(set, assoc, 'i');
        p = &ip[set * assoc + ev];

        p->valid = 1;
        p->time = time_count;
        p->tag = (addr / b_size) / num_of_sets;
    }
    else
    {
        p = &ip[set * assoc + avail];

        p->valid = 1;
        p->time = time_count;
        p->tag = (addr / b_size) / num_of_sets;
    }
}

/* LRU����� ���� victim(��ü ����)�� ���ϴ� �Լ� */
int evict(int set, int assoc, char mode)
{
    int i, tmp_time = 0;                 /* �ݺ����� �ε����� �ð��� �����ϴ� ���� */
    int min = time_count + 1, min_i = 0; /* �ּҰ��� ã�� ���� ������ �ε��� ���� */

    /* set���� time���� ���� ���� ���� �ε����� ã�� return�Ѵ�. */
    for (i = 0; i < assoc; i++)
    {
        if (mode == 'd')
            tmp_time = dp[set * assoc + i].time; // set���� �ֻ��� �ð� ���� �Է�
        if (mode == 'i')
            tmp_time = ip[set * assoc + i].time;
        if (min > tmp_time)
        { // min�� ���ϰ� �� ���� ��� �ش� ȣ��ð����� �����ϰ� �ش���ġ üũ�ϰ�
            min = tmp_time;
            min_i = i;
        }
    }
    return min_i; // ȣ��ð��� ���� �۾Ҵ� ���� ��ġ ��ȯ
}