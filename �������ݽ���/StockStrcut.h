#pragma pack(1)

//  ͷ�ṹ
struct HeaderSection
{
	unsigned int used;
	unsigned __int64 next;
	DataIndex dataIndex[]
};

// 
struct DataIndex
{
	unsigned int time;
	unsigned __int64 start;
};