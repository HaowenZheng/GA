
#include"GeneticAlgorithm.h"//����ͷ�ļ�
//�Ա���ȡֵ��Χ��������Ⱥ��������
const X_Range Range[De_Variable] = { X_Range(-3.0,12.1) ,X_Range(4.1,5.8) };//�Ա��������߻���x1,x2��ȡֵ��Χ
vector<Individual> nowpopulation;//P(t)��Ⱥ
vector<Individual> midpopulation;//�м���Ⱥ���������ѡ�����������
vector<Individual> nextpopulation;//P(t+1)��Ⱥ
//X_Range��ʵ��
X_Range::X_Range(double m_Lower, double m_Upper) :Lower(m_Lower), Upper(m_Upper) {}//X_Range�๹�캯��ʵ��
double X_Range::GetUpper()const//��ȡ��������
{
	return Upper;
}
double X_Range::GetLower()const//��ȡ��������
{
	return Lower;
}
//Individual��ʵ��
Individual::Individual(double* m_Variable)//���캯��
{
	for (int i = 0; i < De_Variable; i++)//��forѭ���Ա��������ֵ
	{
		if (m_Variable[i] >= Range[i].GetLower() && m_Variable[i] <= Range[i].GetUpper())//����Ҫ�����Ա���ȡֵ��Χ�ж�
		{
			Variable[i] = m_Variable[i];//�Ա�����ֵ
		}
		else//������Ҫ���򷢳������沢����
		{
			cerr << "�Ա���ȡֵ������Ҫ��" << endl;
			exit(1);//ֹͣ�����һ�����������ķ�ʽ�����Ա�����ֵ(����ֵ)������˵������ֵ���ڹ涨��Χ��
		}
	}
	//��ʼ��ʱĬ����Ӧֵ��ֵΪ0
	this->Fitness = 0;
	this->ReFitness = 0;
	this->SumFitness = 0;
}
double* Individual::GetVariable()//��ȡ����ֵ
{
	return Variable;
}
double Individual::GetFitness()const//��ȡ��Ӧֵ
{
	return Fitness;
}
double Individual::GetReFitness()const //��ȡ��Ӧֵ����
{
	return ReFitness;
}
double Individual::GetSumFitness()const//��ȡ�ۼӸ���
{
	return SumFitness;
}
void Individual::ChaFitness(const double m_fitness)//�޸���Ӧֵ
{
	this->Fitness = m_fitness;
}
void Individual::ChaReFitness(const double m_ReFitness)//�޸���Ӧֵ����
{
	this->ReFitness = m_ReFitness;
}
void Individual::ChaSumFitness(const double m_SumFitness)//�޸��ۼӸ���
{
	this->SumFitness = m_SumFitness;
}
//�Ŵ��㷨��׼������
void Initialize()//�����ʼ����Ⱥ���õ���һ����Ⱥ
{
	//����ָ����Χ���������������
	double X[Po_Size][De_Variable];//Ϊ��ʹ������������Ԫ������ֵ�ļ��㣬�þ��󱣴���������������ֵ
	for (int j = 0; j < De_Variable; j++)
	{
		default_random_engine e(time(0));//���棬�����������
		uniform_real_distribution<double> u(Range[j].GetLower(), Range[j].GetUpper());//�ֲ�
		for (int i = 0; i < Po_Size; i++)//�Ȱ��д洢�����
		{
			X[i][j] = u(e);//ѭ������ʱ���������ֵ�ͱ�����X������
		}
	}
	//���ɶ���Ⱦɫ�壩�����뵽��ʼ��Ⱥ��
	for (int i = 0; i < Po_Size; i++)
	{
		double variable[De_Variable];
		for (int j = 0; j < De_Variable; j++)
		{
			variable[j] = X[i][j];//���б���
		}
		Individual Indivi(variable);//����һ������Ⱦɫ�壩
		nowpopulation.push_back(Indivi);//���뵽��Ⱥpopulation��
	}
}
void CaculaFitness()//����������Ӧֵ
{
	//f(x1,x2) = 21.5+x1*sin(4pi*x1)+x2*sin(20pi*x2)��Ϊ��Ӧ�ȼ��㺯��
	double fitness = 0;//��ʱ��Ӧֵ
	double x[De_Variable];//��ʱ�洢�Ա���������
	for (int i = 0; i < Po_Size; i++)
	{
		for (int j = 0; j < De_Variable; j++)
			x[j] = nowpopulation.at(i).GetVariable()[j];//������ֱ��
		//fitness = 21.5 + x[0] * sin(4 * PI*x[0]) + 2 * sin(20 * PI*x[1]);//��Ӧ�ȼ���
		fitness = 1 + x[0] * x[0] + x[1] * x[1];
		nowpopulation.at(i).ChaFitness(fitness);//�޸ĵ�ǰȾɫ�����Ӧֵ
	}
}
void CaculaReFitness()//������Ӧֵ����
{
	double sum = 0;//��Ӧֵ�ۼ���
	double temp = 0;
	for (int i = 0; i < Po_Size; i++)//�������Ӧֵ֮��
	{
		sum += nowpopulation.at(i).GetFitness();
	}
	for (int j = 0; j < Po_Size; j++)
	{
		temp = nowpopulation.at(j).GetFitness() / sum;//�������
		nowpopulation.at(j).ChaReFitness(temp);//�޸ĸ������Ӧ�ȸ���
	}
}
void CalculaSumFitness()//�����ۼӸ������
{
	double summation = 0;//�ۼ���
	for (int k = 0; k < Po_Size; k++)
	{
		summation += nowpopulation.at(k).GetReFitness();
		nowpopulation.at(k).ChaSumFitness(summation);//��ǰ�ۼӽ����ֵ
	}
}
void seclect() //��Ⱥѡ��
{
	//���������0��1��С��
	double array[Po_Size];//������������
	default_random_engine e(time(0));//���棬�����������
	uniform_real_distribution<double> u(0.0, 1.0); //�ֲ�
	for (int i = 0; i < Po_Size; i++)
		array[i] = u(e);
	//���̽���ѡ��
	for (int j = 0; j < Po_Size; j++)
	{
		for (int i = 1; i < Po_Size; i++)
		{
			if (array[j] < nowpopulation[i - 1].GetSumFitness())
			{
				midpopulation.push_back(nowpopulation.at(i - 1));//���뵽�м���Ⱥ
			}
			if (array[j] >= nowpopulation.at(i - 1).GetSumFitness() && array[j] <= nowpopulation.at(i).GetSumFitness())
			{
				midpopulation.push_back(nowpopulation.at(i));//���뵽�м���Ⱥ
			}
		}
	}
	nowpopulation.clear();//���nowpopulation
}
double Scand() //�������0��1��С��
{
	int N = rand() % 999;
	return double(N) / 1000.0;;//�������0��1��С��
}
void crossing()//�ӽ�
{
	int num = 0;//��¼����
	double corss = 0.0;//������������ĸ���ֵ
	srand((unsigned)time(NULL));//����ϵͳʱ���������������,����һ��������Ӿ���
	double array1[De_Variable], array2[De_Variable];//��ʱ�洢���׺�ĸ�׵ı���ֵ
	while (num < Po_Size - 1)//����1�����2�ӽ�������3�����4�ӽ�......����i�͸���i+1�ӽ�
	{
		//�ж�˫���Ƿ���Ҫ�ӽ����������һ��0��1��С�����������������ӽ����ʣ�������ӽ���ֱ���Ŵ�����һ�������򣬶Ը�ĸ������ӽ�
		corss = Scand();
		if (corss <= Ov_Probability)//���corssС�ڵ����ӽ�����Ov_Probability�ͽ��е����ӽ�
		{
			//����Ѱ�Ҷ�Ӧ�±�ĸ��岢�ұ���
			for (int i = 0; i < De_Variable; i++)
			{
				array1[i] = midpopulation.at(num).GetVariable()[i];//���׵��Ա���
				array2[i] = midpopulation.at(num + 1).GetVariable()[i];//ĸ���Ա���
			}
			int localx1, localx2;//��¼���򽻲���λ��
			int corssx1[length1], corssx2[length2];//��Ϊ�������������
			double newx1[2], newx2[2];//�ֱ�����������򽻻�������Ӧ�Ա���ֵ
			bool p1 = true, p2 = true;
			//Ȼ���˫�ױ������б��벢�ҽ��е����ӽ�
			for (int j = 0; j < De_Variable; j++)//array1��x1����֮���array2��x1�������е����ӽ����Դ�����
			{
				if (j == 0)//x1���б��벢���ӽ�
				{
					bitset<length1> array1b1((array1[j] + 3.0)* pow(10, 6));//����3.0�γ�һ��unsigaed��֮���ڽ���ĸ��1��x1����
					bitset<length1> array2b1((array2[j] + 3.0)* pow(10, 6));//����3.0�γ�һ��unsigaed��֮���ڽ���ĸ��2��x1����
					//�����������0��length1-1������ȷ��������λ��
					localx1 = rand() % length1;
					//���ڽ��е��㽻�棬����˫��localx1����Ļ���
					for (int i = 0; i < localx1; i++)
						corssx1[i] = array1b1[i];
					for (int k = 0; k < localx1; k++)
						array1b1[k] = array2b1[k];
					for (int s = 0; s < localx1; s++)
						array2b1[s] = corssx1[s];
					//��ֵ������newx1�����У�x1������ɵ����ӽ�����
					newx1[0] = double(array1b1.to_ullong()) / pow(10, 6) - 3.0;
					newx2[0] = double(array2b1.to_ullong()) / pow(10, 6) - 3.0;
					//���²�����ֵ�����жϣ��ж��Ƿ񳬳���Χ�����������Χ���ӽ�
					if (newx1[0]< Range[0].GetLower() || newx1[0]>Range[0].GetUpper() || newx2[0]<Range[0].GetLower() || newx2[0]>Range[0].GetUpper())
					{
						p1 = false;
						break;
					}
				}
				if (j == 1)//x2���б��벢���ӽ�
				{
					bitset<length2> array1b2((array1[j]) * pow(10, 6));//ĸ��1��x2����
					bitset<length2> array2b2((array2[j]) * pow(10, 6));//ĸ��2��x2����
					//�����������0��length2-1������ȷ��������λ��
					localx2 = rand() % length2;
					//���ڽ��е��㽻�棬����˫��localx2����Ļ���
					for (int i = 0; i < localx2; i++)
						corssx2[i] = array1b2[i];
					for (int k = 0; k < localx2; k++)
						array1b2[k] = array2b2[k];
					for (int s = 0; s < localx2; s++)
						array2b2[s] = corssx2[s];
					//��ֵ������newx2�����У�x2������ɵ����ӽ�����
					newx1[1] = double(array1b2.to_ullong()) / pow(10, 6);
					newx2[1] = double(array2b2.to_ullong()) / pow(10, 6);
					//���²�����ֵ�����жϣ��ж��Ƿ񳬳���Χ�����������Χ���ӽ�
					if (newx1[1]< Range[1].GetLower() || newx1[1]>Range[1].GetUpper() || newx2[1]<Range[1].GetLower() || newx2[1]>Range[1].GetUpper())
					{
						p2 = false;
						break;
					}
				}
			}
			if (p1 == true && p2 == true)
			{
				Individual newchiled1(newx1);
				Individual newchiled2(newx2);
				nextpopulation.push_back(newchiled1);
				nextpopulation.push_back(newchiled2);
			}
			else//��ԭ���ĸ����Ŵ�����һ��
			{
				nextpopulation.push_back(midpopulation.at(num));
				nextpopulation.push_back(midpopulation.at(num + 1));
			}
		}
		else//����ֱ���Ŵ�����һ��nextpopulation
		{
			nextpopulation.push_back(midpopulation.at(num));//����һ���µĸ��岢�Ҽ��뵽nextpopulation��
			nextpopulation.push_back(midpopulation.at(num + 1));
		}
		num += 2;
	}
	midpopulation.clear();//���midpopulation
}
void variating()//����
{
	int num = 0;
	while (num < Po_Size)
	{
		double variation = Scand();//�������һ��0��1��С���������ж��Ƿ���б���
		if (variation <= Va_Probability)//���variationС�ڱ���ϵ��������Ҫ���б���
		{
			double x[2];
			bool p = true;
			int x1local, x2local;
			x[0] = nextpopulation.at(num).GetVariable()[0];
			x[1] = nextpopulation.at(num).GetVariable()[1];
			bitset<length1> array1((x[0] + 3.0)* pow(10, 6));//x1����
			bitset<length2> array2(x[1] * pow(10, 6));//x2����
			x1local = rand() % length1;//array1��λȡ��
			x2local = rand() % length2;//array2��λȡ��
			array1.flip(x1local);//�ı�array1 x1localλ��״̬
			array2.flip(x2local);//�ı�array2 x2localλ��״̬
			x[0] = double(array1.to_ullong()) / pow(10, 6) - 3.0;
			x[1] = double(array2.to_ullong()) / pow(10, 6);
			//�ж��Ƿ��������
			if (x[0]< Range[0].GetLower() || x[0]>Range[0].GetUpper() || x[1]<Range[1].GetLower() || x[1]>Range[1].GetUpper())
				p = false;
			if (!p)
				nowpopulation.push_back(nextpopulation.at(num));
			if (p)
			{
				Individual newchiled(x);
				nowpopulation.push_back(newchiled);
			}
		}
		else
			nowpopulation.push_back(nextpopulation.at(num));
		num++;
	}
	nextpopulation.clear();//���nextpopulation
}
void genetic_algorithm()
{
	Initialize();//��ʼ����Ⱥ,������ɵ�һ������
	//����500��B
	for (int i = 0; i < Ev_Algebra; i++)
	{
		CaculaFitness();//��Ӧ�ȼ���
		CaculaReFitness();//��Ӧ�ȸ��ʼ���
		CalculaSumFitness();//�����ۼӸ������
		seclect();//ѡ��
		crossing();//�ӽ�
		variating();//����
	}
	CaculaFitness();//��Ӧ�ȼ���
	double maxfitness = nowpopulation.at(0).GetFitness();
	int maxid = 0;
	int k;
	for (k = 0; k < Po_Size; k++)
	{
		if (maxfitness < nowpopulation.at(k).GetFitness())
		{
			maxfitness = nowpopulation.at(k).GetFitness();
			maxid = k;
		}
	}
	//����500��֮�����
	cout << "x1" << setw(10) << "x2" << setw(15) << "Fitness" << endl;
	for (int j = 0; j < Po_Size; j++)
		cout << nowpopulation.at(j).GetVariable()[0] << setw(10) << nowpopulation.at(j).GetVariable()[1] << setw(10) << nowpopulation.at(j).GetFitness() << endl;
	cout << "x1=" << nowpopulation.at(maxid).GetVariable()[0] << " ��" << "x2=" << nowpopulation.at(maxid).GetVariable()[1] << "ʱȡ�����ֵ��" << maxfitness << endl;
}