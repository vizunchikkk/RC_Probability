#include <iostream>
#include <algorithm>
#include <iomanip>
#include <random>
#include <bitset>

/*��� �� ����� �������� RC5 �� �� ����� ��������� �� 16 ��� � � ������� �������, ��� ������ ����������� �� 32 ��� � ������� ������������
� ��������� 16 ������ ������� � ����� ����� ��������� � �����������
*/
using namespace std;
/*��� ��������� ���� �������� �� ������� ������ ������ ���������, ������� ��������� ��������� ��� � � �������,
��� ���� ����� ��������� ������ ���������*/
const int b = 32; //����� �������� 1 �������� (A-������ � B-�������)
const int l = 16;//����� ���������� ����� � ������ (�� 128 ���)
const int R = 12;//����� �������
const unsigned long P = 0xb7e15163;//� ������ ������� ������������ �����������, ��� ����������� ��������� ����� ����� ��(������� �������)
// �� ���� ���������� 1, ����� ����������� �� 2^b, ����� ���������� �� ���������� ��������� ������
const unsigned long Q = 0x9e3779b9;//����� ������������ ����� �(��������� ������)

//����������� ����� �����
unsigned long left_shift(unsigned long a, unsigned long c)
{
	return (a << c) | (a >> (b - c));
}
//������
unsigned long right_shift(unsigned long a, unsigned long c)
{
	return (a >> c) | (a << (b - c));
}

void code(unsigned long A, unsigned long B, unsigned long* C1, unsigned long* C2, unsigned long* S)
{
	A += S[0];//���������� � 0 ���������
	B += S[1];//���������� � 1 ���������
	//��������� �������� ����������
	for (int i = 1; i <= R; i++)
	{
		A = left_shift((A ^ B), B) + S[i * 2];
		B = left_shift((B ^ A), A) + S[i * 2 + 1];
	}
	*C1 = A, * C2 = B;//� �1 � C2 ��������� ���������
}

void decode(unsigned long A, unsigned long B, unsigned long* D1, unsigned long* D2, unsigned long* S)
{
	//��������� �������� ������������
	for (int i = R; i >= 1; i--)
	{
		B = right_shift((B - S[i * 2 + 1]), A) ^ A;
		A = right_shift((A - S[i * 2]), B) ^ B;
	}
	*D1 = A - S[0], * D2 = B - S[1];//� D1 � D2 ��������� ���������
}

void creating_key(unsigned char* key, unsigned long* S)
{
	int u = b / 8;//����� �������� � ������
	int c = l / u;//����� ���� (����� �����/����� �������� � ��� ��� � ������)
	unsigned long A, B, * L;
	//L-������ ����
	L = (unsigned long*)calloc(c, sizeof(unsigned long));//������ �� ���� �������� ������ ��� �� �� ������� �� ������
	for (int i = l; i <= (c * u - 1); i++)
	{
		key[i] = 0;//��������� ������ ����� �������� ������, ���� �� �������
	}
	//����������� ����� � ������ ����
	L[c - l] = 0;
	for (int i = b - l; i >= 0; i--)
	{
		L[i / u] = left_shift(L[i / u], 8) + key[i];
	}
	S[0] = P;//����������� ���������� ���������
	int t = 2 * (R + 1); //����� ����������� ��������
	for (int i = 1; i < t; i++)
	{
		S[i] = S[i - 1] + Q;//������ �������� � ������� ���������� ���������
	}
	int m = max(t, c);//�������� �������� �� 2 �������� (�� ������� �� 3 �� ��������� ���������)
	//���������� ���������� ����� ������������ �� ��� �������,��������� �������� ���� � ���������
	//������� ������ ����� ��������� ��� ����, � ������ ����� ���� ��������� ������ ���
	for (int i = 0, j = 0, A = B = 0, z = 1; z <= 3 * m; z++)
	{
		A = S[i] = left_shift(S[i] + A + B, 3);
		i = (i + 1) % m;
		B = L[j] = left_shift((L[j] + A + B), (A + B));
		j = (j + 1) % c;
	}
}

int main(void)
{
	setlocale(LC_ALL, "Russian");
	srand(time(NULL));
	unsigned long C1, C2, D1, D2;//���������� ��� �������� ������� ��� ����������� � �������������
	/*
	unsigned char key[l] = {
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00 };
	unsigned long A = 0x0000, B = 0x0000;

	unsigned char key[l] = {
		0x91, 0x5f, 0x46, 0x19,
		0xbe, 0x41, 0xb2, 0x51,
		0x63, 0x55, 0xa5, 0x01,
		0x10, 0xa9, 0xce, 0x91 };
	unsigned long A = 0xeedb, B = 0x6d8f;

	unsigned char key[l] = {
	0x78, 0x33, 0x48, 0xe7,
	0x5a, 0xeb, 0x0f, 0x2f,
	0xd7, 0xb1, 0x69, 0xbb,
	0x8d, 0xc1, 0x67, 0x87 };
	unsigned long A = 0xac13, B = 0x5289;

	unsigned char key[l] = {
		0xdc, 0x49, 0xdb, 0x13,
		0x75, 0xa5, 0x58, 0x4f,
		0x64, 0x85, 0xb4, 0x13,
		0xb5, 0xf1, 0x2b, 0xaf };
	unsigned long A = 0xb7b3, B = 0x92fc;


	unsigned char key[l] = {
		0x52, 0x69, 0xf1, 0x49,
		0xd4, 0x1b, 0xa0, 0x15,
		0x24, 0x97, 0x57, 0x4d,
		0x7f, 0x15, 0x31, 0x25 };
	unsigned long A = 0xb278, B = 0xcc97;

	*/
	//��� ����
	//�-����� ����� ��������, �-������
	unsigned char key[l] = {
		0x52, 0x69, 0xf1, 0x49,
		0xd4, 0x1b, 0xa0, 0x15,
		0x24, 0x97, 0x57, 0x4d,
		0x7f, 0x15, 0x31, 0x25 };
	unsigned long A = 0xb278, B = 0xcc97;
	cout << "��������� ������" << endl;
	cout << hex << A << "  " << B << endl;

	//���������� ������ ���� ������
	//���������� ��������� mersenne twister, �� �� ������ � ������ ��������,�� ����������� 32����
	std::mt19937 genA(std::random_device{}());
	std::mt19937 genB(std::random_device{}());
	unsigned long A1 = unsigned short(genA());//����� �������� ��� ������ 16 ���
	unsigned long B1 = unsigned short(genB());
	cout << "��������� �������� ������" << endl;
	cout << hex << A1 << "  " << B1 << endl;
	A = A << (b / 2) | A1;//��������(������������) ����� (������)
	B = B << (b / 2) | B1;
	cout << "�������� ��������� � �������" << endl;
	cout << hex << A << "  " << B << endl;


	unsigned long S[2 * (R + 1)];//���������� ��� ������ ����������� ���������
	creating_key(key, S);//����� ��������� ��������
	code(A, B, &C1, &C2, S);
	cout << "�������������" << endl;
	cout << hex << C1 << "  " << C2 << endl;
	decode(C1, C2, &D1, &D2, S);
	cout << "��������������" << endl;//�������� ���� ����� � 1 �������� �����
	cout << hex << D1 << "  " << D2 << endl;
	D1 = D1 >> (b / 2);//�������� ������ �������� � ������, �� ��� ���� ���������
	D2 = D2 >> (b / 2);
	cout << "���������� ������(��� ��������)" << endl;
	cout << hex << D1 << "  " << D2 << endl;
	system("pause");
	return 0;
}