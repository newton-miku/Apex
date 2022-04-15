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
	const std::string alpha_numeric(E("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890���ɴӵĶ���;Ϳ�������Ŷ��ȥ��������������ﹰ�߹�ĺ��H�����������ѱ����ȱ𱾰�ʽ�̶����¼��ۿ�����ִ������ɨ���ﳡ����â��֥���ӻ�Ȩ������Э��ѹ�����аٴ��ҳ�����Ҵ������ɼй�а������������ʦ�����ӹ⵱��	���ų�����ͬ�����������췫�����������������ȶ�����Ǩ��ΰ��ƹ��������ŷ��Ӽ����˼۷ݻ����»�α��Ѫ���ƺ�����ȫ��ɱ��������үɡ��������ΣѮּ����������ɫ׳���ׯ�������뽻���²����������ʴ��򲢹��׵��ݺ��۽�����æ������լ�ְ���������ũ�����Ѱ��Ѹ�������������ս��������績������Ϸ��ۻ�����˼�Լ�ͳ�Ѳ"));

	std::default_random_engine generator{ std::random_device{}() };
	const std::uniform_int_distribution< std::string::size_type > distribution{ 0, alpha_numeric.size() - 1 };

	std::string str(len, 0);
	for (auto& it : str)
		it = alpha_numeric[distribution(generator)];

	return str;
}