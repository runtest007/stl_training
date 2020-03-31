/*============================================================================================================================================
1)  某市举行一场演讲比赛（ speech_contest ），共有24个人参加。比赛共三轮，前两轮为淘汰赛，第三轮为决赛。

2)  比赛方式：分组比赛，每组6个人；选手每次要随机分组，进行比赛；
        a)第一轮分为4个小组，每组6个人。每人分别按照抽签（draw）顺序演讲。当小组演讲完后，淘汰组内排名最后的三个选手，然后继续下一个小组的比赛。
        b)第二轮分为2个小组，每组6人。比赛完毕，淘汰组内排名最后的三个选手，然后继续下一个小组的比赛。
        c)第三轮只剩下6个人，本轮为决赛，选出前三名。

3） 比赛评分：每个选手演讲完由10个评委打分，去除最低、最高分，求得剩下的8个成绩的平均分,选手的名次按得分降序排列，若得分一样，按参赛号升序排名。

4)用STL编程，求解这个问题
    1）	请打印出所有选手的名字与参赛号，并以参赛号的升序排列。
    2）	打印每一轮比赛后，小组比赛成绩和小组晋级名单
    3）	打印决赛前三名，选手名称、成绩。
==============================================================================================================================================*/
#include<stdio.h>
#include<iostream>
#include<iterator>
#include<string>
#include<vector>
#include<deque>
#include<map>
#include<algorithm>
#include<functional>
#include<numeric>
using namespace std;


//演讲选手结构体
class Speaker 
{
public:
	string	m_name;
	int		m_score[3];
};

//产生选手
int GenSpeaker(map<int,Speaker> &mapSpeaker,vector<int> &v)
{
	string str = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	random_shuffle(str.begin(),str.end());

    for(int i=0;i<24;i++)
	{
		Speaker temp;
		string s = "选手";
		temp.m_name = s+str[i];
		mapSpeaker.insert(pair<int,Speaker>(100+i,temp));    
		v.push_back(100+i);//参加比赛的人员
	}
    
	return 0;
}

//产生抽签
int speech_contest_draw(vector<int> &v)
{
	random_shuffle(v.begin(),v.end());
	return 0;
}

//选手比赛
int speech_contest(int index,vector<int> &v1,map<int,Speaker> &mapSpeaker,vector<int> &v2)
{
    int count = 0;
	multimap<int, int, greater<int> > multmapGroup;
	
	for(vector<int>::iterator it=v1.begin(); it!=v1.end(); it++)
	{
		count++;

        //打分
		{
			deque<int> dscore;
            //十个评委打分
			for(int j=0; j<10; j++)
			{
				int score = 50+rand()%50;//用了一个随机数种子
				dscore.push_back(score);
			}
            //分数排序
			sort(dscore.begin(),dscore.end());
            //去掉最高 最低分
            dscore.pop_back();
			dscore.pop_front();
		    //求平均分
			int scoresum = accumulate(dscore.begin(),dscore.end(),0);
			int scoreavg= scoresum/dscore.size();
			mapSpeaker[*it].m_score[index] = scoreavg;//选手得分 存入容器中
			multmapGroup.insert(pair<int,int>(scoreavg,*it));
		}
        
		if(count % 6 == 0)//按六个参赛成员分为一个小组并打印成绩
		{
			cout<<"小组的比赛成绩"<<endl;
			for(multimap<int,int,greater<int> >::iterator it=multmapGroup.begin();it!=multmapGroup.end();++it)
			{
				//编号 姓名 得分
				cout<<"#"<<it->second<<"\t"<<mapSpeaker[it->second].m_name<<"\t"<<it->first<<endl;
			}
            
			//把小组前三名放到下一轮参赛的vector容器中
			while(multmapGroup.size()>3)
			{
				multimap<int,int,greater<int> >::iterator it = multmapGroup.begin();
				v2.push_back(it->second);
				multmapGroup.erase(it);
			}
			multmapGroup.clear();//清除本小组成绩
		}
	}
	return 0;
}

//打印每一轮晋级名单
int speech_contest_print(int index,vector<int> &v,map<int,Speaker> &mapSpeaker)
{	
	printf("第%d轮 晋级名单\n",index+1);
	for(vector<int>::iterator it = v.begin();it!=v.end();++it)
	{
		cout<<"#"<<*it<<"\t"<<mapSpeaker[*it].m_name<<"\t"<<mapSpeaker[*it].m_score[index]<<endl;
	}
	return 0;
}
 
int main()
{
	//容器的设计
	map<int,Speaker>	mapSpeaker; //参加选手的id，参加比赛的选手 
	vector<int>			v1;         //第一轮演讲比赛名单
	vector<int>			v2;         //第二轮演讲比赛名单
	vector<int>			v3;         //第三轮演讲比赛名单
	vector<int>			v4;         //最后前三名 演讲比赛名单
 
	//产生选手 得到第一轮选手的比赛名单
	GenSpeaker(mapSpeaker,v1);
 
	//第一轮 选手抽签 选手比赛 查看比赛结果
	cout<<"按确认键开始第1轮比赛"<<endl;
	cin.get();
	speech_contest_draw(v1);
	speech_contest(0,v1,mapSpeaker,v2);
	speech_contest_print(0,v2,mapSpeaker);

    //第二轮 选手抽签 选手比赛 查看比赛结果
	cout<<"\n\n\n确认键开始第2轮比赛"<<endl;
	cin.get();
	speech_contest_draw(v2);
	speech_contest(1,v2,mapSpeaker,v3);
	speech_contest_print(1,v3,mapSpeaker);

    //第三轮 选手抽签 选手比赛 查看比赛结果
	cout<<"\n\n\n确认键开始第3轮比赛"<<endl;
	cin.get();
	speech_contest_draw(v3);
	speech_contest(2,v3,mapSpeaker,v4);
	speech_contest_print(2,v4,mapSpeaker);
	
   return 0;
}
