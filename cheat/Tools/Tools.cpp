#include "tools.h"


uint32_t get_process_pid(const char* process_name)
{
	PROCESSENTRY32 process_entry{};
	process_entry.dwSize = sizeof(PROCESSENTRY32);
	uint32_t pid = 0;
	auto snapshot{ CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL) };
	if (snapshot == INVALID_HANDLE_VALUE)
		return false;
	if (Process32First(snapshot, &process_entry))
	{
		do {
			if (!strcmp(process_name, process_entry.szExeFile))
			{
				pid = process_entry.th32ProcessID;
				CloseHandle(snapshot);
				return pid;
			}
		} while (Process32Next(snapshot, &process_entry));
	}
	CloseHandle(snapshot);
	return 0;
}

bool RenameFile(std::string& path)
{
	std::string newPath = (RandomString(16) + ".exe");
	SetConsoleTitleA(RandomString(16).c_str());
	if (std::rename(path.c_str(), newPath.c_str())) return false;

	path = newPath;

	return true;
}

std::string RandomString(const int len)
{
	const std::string alpha_numeric(E("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890啊吧从的额发给和就看了吗你哦怕去人是他我下有在锕阿吖嗄呵錒哎爱按唉不把被并比别本八式刑动扛寺吉扣考托老执巩圾扩扫地扬场耳共芒亚芝朽朴机权过臣再协西压厌在有百存而页匠夸夺灰达列死成夹轨邪划迈毕至此贞师尘尖劣光当早	吐吓虫曲团同吊吃因吸吗屿帆岁回岂刚则肉网年朱先丢舌竹迁乔伟传乒乓休伍伏优伐延件任伤价份华仰仿伙伪自血向似后行舟全会杀合兆企众爷伞创肌朵杂危旬旨负各名多争色壮冲冰庄庆亦刘齐交次衣产决充妄闭问闯羊并关米灯州汗污江池汤忙兴宇守宅字安讲军许论农讽设访寻那迅尽导异孙阵阳收阶阴防奸如妇好她妈戏羽观欢买红纤级约纪驰巡"));

	std::default_random_engine generator{ std::random_device{}() };
	const std::uniform_int_distribution< std::string::size_type > distribution{ 0, alpha_numeric.size() - 1 };

	std::string str(len, 0);
	for (auto& it : str)
		it = alpha_numeric[distribution(generator)];

	return str;
}