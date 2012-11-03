#include "Log.h"

namespace Log
{
	void Write::logWrite(String^ str,String^ reason,String^ logname)
	{
		String^ EntryTime = (gcnew DateTime())->Now.ToLongTimeString();
		String^ EntryDate = (gcnew DateTime())->Today.ToShortDateString();

		try
		{
			if(!Directory::Exists(Environment::CurrentDirectory+"/log/"))
			{
				Directory::CreateDirectory((Environment::CurrentDirectory+"/log/"));
			}

			String^ fileName = "log/"+logname+".log";
			StreamWriter^ sw = gcnew StreamWriter(fileName,true,System::Text::Encoding::UTF8);
			sw->WriteLine("["+EntryDate+"]["+EntryTime+"]["+reason+"]"+" "+str);
			sw->Close();
		}
		catch (Exception^ exc)
		{
			MessageBox::Show(exc->Message);
		}
	}

}