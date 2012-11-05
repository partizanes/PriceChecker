#include "Log.h"
#include "Form1.h"
#include <windows.h>

#using <md5.dll>


bool pass = false;
int last_image_num = 0;

using namespace Md5;
using namespace Log;
using namespace System::IO;
using namespace System::Net;
using namespace System::Data;
using namespace PriceChecker;
using namespace System::Net::Sockets;
using namespace System::Globalization;
using namespace System::Runtime::InteropServices;
using namespace System::Text::RegularExpressions;
using namespace System::Runtime::Serialization::Formatters::Binary;

#pragma comment(lib,"User32.lib")

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	if (System::Diagnostics::Process::GetProcessesByName(Application::ProductName)->Length > 1)
	{
		MessageBox::Show("Приложение уже запущено!");
		return 0;
	}
	else
	{
		Application::EnableVisualStyles();
		Application::SetCompatibleTextRenderingDefault(false);

		try
		{
			Application::Run(gcnew Form1());
			return 0;
		}
		catch (Exception^ exc)
		{
			Write::logWrite(exc->Message,"EXCEPTION","pc");
		}
	}
}

Void Form1::Form1_Load(System::Object^  sender, System::EventArgs^  e)
{
	barcode_text_box->Focus();

	label_version_parametr->Text = Convert::ToString(version);

	//Hide cursor
	Cursor->Hide();

	//label
	old_price_label->Visible = false;
	action_label->Visible = false;
	msg_label->Visible = false;
	panel4->Visible = false;
	weight_label->Visible = false;
	total_label->Visible = false;

	//para
	old_price_para->Visible = false;
	price_para->Visible = false;
	weight_para->Visible = false;
	total_para->Visible = false;

	//mb this not need
	pictureBox1->BackColor = Color::Transparent;

	//full screen size after form load
	Form1::Size = System::Drawing::Size(GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
	Form1::CenterToScreen();

	//lineshape
	lineShape1->Visible = false;
	lineShape2->Visible = false;

	//testing system
	char buf[50];

	//timer for random image
	timer1->Interval = (GetPrivateProfileInt("SETTINGS", "random_img_interval",5,SystemStringToChar(Environment::CurrentDirectory+"\\config.ini")))*1000;
	timer1->Enabled = true;

	Write::logWrite("Терминал запущен!","SYSTEM","pc");

	//logo after load
	String^ path= String::Format("{0}\\image\\logo.jpg",Application::StartupPath);
	pictureBox1->ImageLocation = path;
	pictureBox1->SizeMode = PictureBoxSizeMode::StretchImage;

	//set upload_log_interval from config default 3hours
	log_upload_timer->Interval = (GetPrivateProfileInt("SETTINGS", "upload_log_interval",3,SystemStringToChar(Environment::CurrentDirectory+"\\config.ini")))*3600000;
	log_upload_timer->Enabled = true;

	DirectoryInfo^ directoryInfo = gcnew DirectoryInfo(Environment::CurrentDirectory+"\\image\\");

	//enable autoupdate
	auto_update_timer->Interval = (GetPrivateProfileInt("SETTINGS", "check_update",1,SystemStringToChar(Environment::CurrentDirectory+"\\config.ini")))*3600000;
	auto_update_timer->Enabled = true;

	//check number files
	if (directoryInfo->Exists)
		last_image_num += directoryInfo->GetFiles("*.jpg", SearchOption::TopDirectoryOnly)->Length;
	else
		last_image_num = GetPrivateProfileInt("SETTINGS", "last_image_num",1,SystemStringToChar(Environment::CurrentDirectory+"\\config.ini"));

	GetPrivateProfileString("SETTINGS", "srv_global","192.168.1.100",buf,sizeof(buf),SystemStringToChar(Environment::CurrentDirectory+"\\config.ini"));

	connStr = String::Format("server={0};uid={1};pwd={2};database={3};",
		CharToSystemString(buf), "admin", "12345", "ukmserver");

	server1Conn = gcnew MySqlConnection(connStr);

	GetPrivateProfileString("SETTINGS", "srv_local","192.168.1.11",buf,sizeof(buf),SystemStringToChar(Environment::CurrentDirectory+"\\config.ini"));

	connStr = String::Format("server={0};uid={1};pwd={2};database={3};",
		CharToSystemString(buf), "pricechecker", "7194622Parti", "action");

	server2Conn = gcnew MySqlConnection(connStr);

	diag_system();

	CheckVersion();

	set_msg_on_timer( "Версия Приложения:" + version );
}

Void Form1::barcode_text_box_TextChanged(System::Object^  sender, System::EventArgs^  e)
{
	//THIS TEST FUNCTION FOR SUPPORT UPC BARCODE
	if(barcode_text_box->Text->Length == 12)
	{
			if((barcode_text_box->Text[0]- '0') == 0)
			{
				String^ bar;

				for (int i=0; i< barcode_text_box->Text->Length; i++)
				{
					bar += this->barcode_text_box->Text[i];
				}

				query("0"+bar);
				Write::logWrite("0"+bar,"DEBUG","pc");
			}
	}
}

Void Form1::timer1_Tick(System::Object^  sender, System::EventArgs^  e)
{
	try
	{
		Random^ rnd=gcnew Random();
		int i=rnd->Next(last_image_num);

		DirectoryInfo^ di = gcnew DirectoryInfo( Application::StartupPath + "\\image" );

		array<FileInfo^>^fiArr = di->GetFiles("*.jpg");

		while(fiArr[i]->Name == "logo.jpg")
			i=rnd->Next(last_image_num);

		String^ path= String::Format("{0}\\image\\{1}",Application::StartupPath,(fiArr[i]->Name));

		if(!File::Exists(path))
		{
			String^ path= String::Format("{0}\\image\\logo.jpg",Application::StartupPath);
			pictureBox1->ImageLocation = path;
			pictureBox1->SizeMode = PictureBoxSizeMode::StretchImage;

			Write::logWrite("Изображение не найдено: "+path,"SYSTEM","pc");
			return;
		}

		pictureBox1->ImageLocation = path;
		pictureBox1->SizeMode = PictureBoxSizeMode::StretchImage;
	}

	catch (Exception^ exc)
	{
		Write::logWrite(exc->Message,"EXCEPTION","pc");
		set_msg_on_timer("Exception: " + exc->Message);
	}
}

Boolean Form1::ean13_validate(int barcode[])
{
	Boolean check = false;

	Int32 summ = ((barcode[1]- '0')+(barcode[3]- '0')+(barcode[5]- '0')+(barcode[7]- '0')+(barcode[9]- '0')+(barcode[11]- '0'))*3+((barcode[0]- '0')+(barcode[2]- '0')+(barcode[4]- '0')+(barcode[6]- '0')+(barcode[8]- '0')+(barcode[10]- '0'));
	Int32 ten = summ/10;
	summ = 10-(summ - (ten*10));

	if(summ == (barcode[12]- '0'))
		check = true;
	else	
		check = false;

	return check;
}

Boolean Form1::ean8_validate(int barcode[])
{
	Boolean check = false;

	Int32 summ = (barcode[0]- '0')*3+(barcode[1]- '0')+(barcode[2]- '0')*3+(barcode[3]- '0')+(barcode[4]- '0')*3+(barcode[5]- '0')+(barcode[6]- '0')*3+(barcode[7]- '0');
	Int32 ten = summ/10;


	if(summ - (ten*10) == 0)
		check = true;
	else
		check = false;

	return check;
}

Void Form1::query(String^ bar)
{
	MySqlDataReader^ reader = nullptr;

	try
	{
		server1Conn->Open();

		char buf[14];
		GetPrivateProfileString("SETTINGS", "id_pricelist","1",buf,sizeof(buf),SystemStringToChar(Environment::CurrentDirectory+"\\config.ini"));

		cmd = gcnew MySqlCommand("SELECT a.name, b.price \n"
			 "FROM trm_in_var C \n"
			 "LEFT JOIN trm_in_items A ON A.id=C.item \n"
			 "LEFT JOIN trm_in_pricelist_items B ON B.item=c.item \n"
			 "WHERE C.id='"+bar+"' \n"
			 "AND (b.pricelist_id="+CharToSystemString(buf)+")", server1Conn);

		MySqlDataReader^ reader = cmd->ExecuteReader();

		if(reader->Read())
		{
			price_para->Visible = true;
			item_name_textbox->Text = reader->GetString(0);
			price_para->Text =Convert::ToString(reader->GetInt32(1));
			barcode_text_box->Text = "";
			msg_label->Text = "";
			panel4->Visible = false;

			picture_off();

			action_check(bar);
		}
		else
		{
			Write::logWrite("Не найден "+bar,"SYSTEM","pc");
			barcode_text_box->Text = "";
			set_msg_on_timer("Штрих-код не найден!Обратитесь к продавцу!");
		}
	}
	catch (Exception^ exc)
	{
		Write::logWrite(exc->Message,"EXCEPTION","pc");
		set_msg_on_timer("Exception: " + exc->Message);

		//may be this need to clear barcode_text_box after Exception
		//test this
		barcode_text_box->Text = "";
	}
	finally
	{
		delete cmd;

		if (reader != nullptr)
			reader->Close();

		if (server1Conn->State == ConnectionState::Open)
			server1Conn->Close();
	}
}

Void Form1::set_msg_on_timer(String^ text)
{
	panel4->Visible = true;
	msg_clear->Enabled = true;
	msg_label->Visible = true;
	msg_label->Text = text;
}

Void Form1::barcode_text_box_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
{
	if (e->KeyCode == Keys::Enter)
	{
		int len = barcode_text_box->TextLength ;
		Int32 barcode[13];

		String^ bar;
		String^ weight;

		old_price_label->Visible = false;
		old_price_para->Visible = false;
		action_visible->Enabled = false;
		action_label->Visible = false;

		old_price_para->Text = "";
		lineShape1->Visible = false;
		lineShape2->Visible = false;

		switch (len)
		{
		case 0:
			break;
		case 1:
			for (int i=0; i< len; i++)
			{
				barcode[i] = this->barcode_text_box->Text[i];
			}
			if((barcode[0]- '0') == 1)
			{
				set_msg_on_timer("Тестирование системы");
				stg_panel->Visible = true;
				barcode_text_box->Text = "";
			}
			else
			{
				barcode_text_box->Text = "";
				set_msg_on_timer("Данные не верны!");
			}
			break;
		case 2:
		case 3:
			break;
		case 4:
			for (int i=0; i< len; i++)
			{
				barcode[i] = this->barcode_text_box->Text[i];
			}

			if((barcode[0]- '0') == 9 && (barcode[1]- '0') == 9 && (barcode[2]- '0') == 9 && (barcode[3]- '0') == 9)
			{
				pass = true;
				msg_label->Visible = true;
				panel4->Visible = true;
				msg_label->Text = "Проверка Пароля...";
				barcode_text_box->Text = "";
				barcode_text_box->UseSystemPasswordChar = true ;
				pass_timer->Enabled = true;
			}

			if((barcode[0]- '0') == 0 && (barcode[1]- '0') == 0 && (barcode[2]- '0') == 0 && (barcode[3]- '0') == 0)
			{
				Application::Exit();
			}

			break;
		case 5:
			//короткие коды на весовые товары(+прочие) ,цена за килограмм(цена за штуку)
			//практическое применение:сверка продавцами ценников и цен на кассе с помощью прайсчекера.
			//вынужденое использование отдельного запроса
			//тестовая функция

			for (int i=0; i< len; i++)
			{
				barcode[i] = this->barcode_text_box->Text[i];
			}

			for (int i=0; i< len; i++)
			{
				bar += this->barcode_text_box->Text[i];
			}

			queryfive(bar);
			    break;

		case 8:
			for (int i=0; i< len; i++)
			{
				barcode[i] = this->barcode_text_box->Text[i];
			}
			if(ean8_validate(barcode))
			{
				for (int i=0; i< len; i++)
				{
					bar += this->barcode_text_box->Text[i];
				}
				query(bar);
				    break;
			}
			else
			{
				set_msg_on_timer("Возможно,штрих-код был считан неверно,попробуем найти...");

				for (int i=0; i< len; i++)
				{
					bar += this->barcode_text_box->Text[i];
				}

				query(bar);

				Write::logWrite(bar,"NOTVALID[EAN8]","nv");

				    break;
			}
		case 10:
			if(!pass)
			{
				set_msg_on_timer("Авторизация не пройдена!");
				Write::logWrite("false","PASS","auth");
				barcode_text_box->Text = "";
				break;
			}
			for (int i=0; i< len; i++)
			{
				barcode[i] = this->barcode_text_box->Text[i];
			}

			if((barcode[0]- '0') == 9 && (barcode[1]- '0') == 9 && (barcode[2]- '0') == 9 && (barcode[3]- '0') == 9 && (barcode[4]- '0') == 9 && (barcode[5]- '0') == 9 && (barcode[6]- '0') == 9 && (barcode[7]- '0') == 9 && (barcode[8]- '0') == 9  && (barcode[9]- '0') == 9) 
			{
				set_msg_on_timer("                                       Настройки");
				barcode_text_box->Text = "";
				stg_panel->Visible = true;
				Write::logWrite("true","PASS","auth");
				break;
			}
			else
			{
				pass = false;
				pass_timer->Enabled = false;
				set_msg_on_timer("Неправильный пароль!");
				barcode_text_box->UseSystemPasswordChar = false ;
				break;
			}

		case 13:

			for (int i=0; i< len; i++)
			{
				barcode[i] = this->barcode_text_box->Text[i];
			}

			if((barcode[0]- '0') == 2 && (barcode[1]- '0') == 0)
			{
				for (int i=0; i< 7; i++)
				{
					bar += this->barcode_text_box->Text[i];
				}

				for (int i=7; i< 12; i++)
				{
					weight += this->barcode_text_box->Text[i];
				}

				if (Convert::ToInt32(weight) == 0)
				{
					barcode_text_box->Text = "";
					set_msg_on_timer ("Вес товара не может быть равен нулю!") ;
					Write::logWrite(weight,"WEIGHT","pc");
					return;
				}


				query(bar);

				//MB THIS NEED SYSTEM MESSAGE TO ADMINISTRATOR
				if(price_para->Text == "Price")
				{
					set_msg_on_timer ("Товар не найден!!!") ;
					Write::logWrite("Ошибка!Товар не прогружен! ->"+bar,"NOTFOUND","pc");
					return;
				}

				float w = (float(Convert::ToInt32(weight))/1000);

				weight_para->Text = Convert::ToString(w);

				w = float((Convert::ToInt32(price_para->Text))*(w));

				weight_para->Visible = true;
				weight_label->Visible = true;
				total_para->Visible = true;
				total_label->Visible = true;
				total_para->Text = Convert::ToString(Convert::ToInt32(w));
				weight_clr->Enabled = true;
				set_msg_on_timer("Цена указана за килограмм,цена вашей покупки составит "+ Convert::ToString(Convert::ToInt32(w)));

				    break;
			}
			if(ean13_validate(barcode))
			{
				for (int i=0; i< len; i++)
				{
					bar += this->barcode_text_box->Text[i];
				}

				query(bar);
			}
			else
			{
				set_msg_on_timer("Возможно,штрих-код был считан неверно,попробуем найти...");

				for (int i=0; i< len; i++)
				{
					bar += this->barcode_text_box->Text[i];
				}

				query(bar);
				Write::logWrite(bar,"NOTVALID[EAN13]","nv");
			}
			break;

		}
	}
}

Void Form1::pass_timer_Tick(System::Object^  sender, System::EventArgs^  e)
{
	pass = false;
	msg_label->Visible = false;
	panel4->Visible = false;
	msg_label->Text = "";
	barcode_text_box->UseSystemPasswordChar = false ;
	pass_timer->Enabled = false;
}

Void Form1::diag_system()
{
	char buf[5];

	GetPrivateProfileString("SETTINGS", "start_check","true",buf,sizeof(buf),SystemStringToChar(Environment::CurrentDirectory+"\\config.ini"));

	if(CharToSystemString(buf) != "true")
		return;

	boolean off = false;

	stg_panel->Visible = true;
	panel4->Visible = true;
	msg_label->Visible = true;
	msg_label->Text = "Диагностика...";
	dir_exist_para->Text = "Проверка...";
	mysql_check_para->Text = "Проверка...";

	if(Directory::Exists(Environment::CurrentDirectory+"/image/"))
	{
		dir_exist_para->Text = "Доступно!";
		dir_exist_para->ForeColor = Color::Green ;
	}

	if(!Directory::Exists(Environment::CurrentDirectory+"/image/"))
	{
		dir_exist_para->ForeColor = Color::Red ;
		dir_exist_para->Text = "Не доступно!";

		String^ message = "Папка с рекламой не обнаружена,проверить еще раз?";
		String^ caption = "Ошибка";

		MessageBoxButtons buttons = MessageBoxButtons::YesNo;
		System::Windows::Forms::DialogResult result;

		result = MessageBox::Show( this, message, caption, buttons );

		if ( result == ::DialogResult::Yes )
		{
			diag_system();
		}
		else
		{
			Application::Exit();
		}
	}

	if(mysqlcheck())
	{
		mysql_check_para->Text = "Доступно!";
		mysql_check_para->ForeColor = Color::Green ;

		off = true;
	}
	if(!mysqlcheck())
	{
		mysql_check_para->ForeColor = Color::Red ;
		mysql_check_para->Text = "Не доступно!";

		String^ message = "Не удалось соединиться с Mysql сервером,попробывать еще раз?";
		String^ caption = "Ошибка";

		MessageBoxButtons buttons = MessageBoxButtons::YesNo;
		System::Windows::Forms::DialogResult result;

		result = MessageBox::Show( this, message, caption, buttons );

		if ( result == ::DialogResult::Yes )
		{
			mysql_check_para->Text = "Проверка...";
			diag_system();
		}
		else
		{
			//TODO Предложить изменить настройки сервера.
			Application::Exit();
		}
	}
	if(off)
	{
		panel4->Visible = false;
		msg_label->Visible = false;
		msg_label->Text = "";
		stg_panel->Visible = false;
	}
}

Boolean Form1::mysqlcheck()
{
	MySqlDataReader^ reader = nullptr;

	try
	{
		server1Conn->Open();
		return true;
	}
	catch (Exception^ exc)
	{
		Write::logWrite(exc->Message,"EXCEPTION","pc");
		set_msg_on_timer("Exception: " + exc->Message);
		return false;
	}
	finally
	{
		if (reader != nullptr)
			reader->Close();

		if (server1Conn->State == ConnectionState::Open)
			server1Conn->Close();
	}

}

Void Form1::close_menu_Click(System::Object^  sender, System::EventArgs^  e)
{
	stg_panel->Visible = false;
	msg_label->Visible = false;
	panel4->Visible = false;
	msg_label->Text = "";

}

Void Form1::test_button_Click(System::Object^  sender, System::EventArgs^  e)
{
	dir_exist_para->Text = "Проверка...";
	mysql_check_para->Text = "Проверка...";

	diag_system();

	stg_panel->Visible = true;
	panel4->Visible = true;
	msg_label->Visible = true;

	msg_label->Text = "                       Тестирование системы";
}

Void Form1::action_check(String^ bar)
{
	MySqlDataReader^ reader = nullptr;

	try
	{
		server2Conn->Open();

		CultureInfo^ jaJP = gcnew CultureInfo("ja-JP");

		String^ EntryDate =  (gcnew DateTime())->Today.ToString("d",jaJP)->Replace("/","-");

		cmd = gcnew MySqlCommand("SELECT price_old,price_new FROM action_price WHERE barcode = "+bar+" AND price_new = "+price_para->Text+" AND start_action <= '"+EntryDate+"' AND stop_action >= '"+EntryDate+"'", server2Conn);

		MySqlDataReader^ reader = cmd->ExecuteReader();

		if(reader->Read())
		{
			action_label->Visible = true;
			action_visible->Enabled = true;
			old_price_label->Visible = true;
			lineShape1->Visible = true;
			lineShape2->Visible = true;
			old_price_para->Visible = true;
			old_price_para->Text = reader->GetString(0);
		}
	}
	catch (Exception^ exc)
	{
		Write::logWrite(exc->Message,"EXCEPTION","pc");
		set_msg_on_timer("Exception: " + exc->Message);
	}
	finally
	{
		delete cmd;

		if (reader != nullptr)
			reader->Close();

		if (server2Conn->State == ConnectionState::Open)
			server2Conn->Close();
	}
}

char* Form1::SystemStringToChar(System::String^ string)
{
	return (char*)(void*)Marshal::StringToHGlobalAnsi(string);
}

String^ Form1::CharToSystemString(char* ch)
{
	char * chr=new char[]=ch;
	System::String^ str;
	for(int i=0;chr[i]!='\0';i++)
	{
		str+=wchar_t(ch[i]);
	}
	return str;
}

Void Form1::opt_button_Click(System::Object^  sender, System::EventArgs^  e)
{
	MySqlDataReader^ reader = nullptr;

	try
	{
		server2Conn->Open();

		cmd = gcnew MySqlCommand("OPTIMIZE TABLE action_price", server2Conn);

		MySqlDataReader^ reader = cmd->ExecuteReader();

		if(reader->Read())
		{
			set_msg_on_timer(reader->GetString(3));
		}
		else
		{
			set_msg_on_timer("                       Ошибка!");
		}
		while(reader->Read())
		{
			set_msg_on_timer("                       "+reader->GetString(3));
		}

	}
	catch (Exception^ exc)
	{
		Write::logWrite(exc->Message,"EXCEPTION","pc");
		set_msg_on_timer("Exception: " + exc->Message);
	}
	finally
	{
		delete cmd;

		if (reader != nullptr)
			reader->Close();

		if (server2Conn->State == ConnectionState::Open)
			server2Conn->Close();
	}
}

Void Form1::upload_button_Click(System::Object^  sender, System::EventArgs^  e)
{
	upload_button->Enabled = false;
	backgroundWorker1->RunWorkerAsync();
}

Void Form1::backgroundWorker1_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e)
{
	String^ file_name;
	int okey = 0;
	int t_log;

	DirectoryInfo^ dinf = gcnew DirectoryInfo(Environment::CurrentDirectory+"\\log\\");

	if (dinf->Exists)
		t_log = dinf->GetFiles("*.log", SearchOption::TopDirectoryOnly)->Length;

	array<FileInfo^>^f_inf = dinf->GetFiles("*.log");

	for (int i=0; i < t_log; i++)
	{
		file_name = f_inf[i]->Name;

		 try
		 {
			 FileStream^ fs = File::OpenRead( Environment::CurrentDirectory+"/log/"+file_name );
			 array<Byte>^ buffer = gcnew array<Byte>((int)fs->Length);
			 int len = safe_cast<int>(fs->Length);
			 fs->Read(buffer,0,len);
			 fs->Close();

			 char buf[17];

			 GetPrivateProfileString("SETTINGS", "upload_server","error",buf,sizeof(buf),SystemStringToChar(Environment::CurrentDirectory+"\\config.ini"));

			 if(CharToSystemString(buf) == "error")
			 {
				 set_msg_on_timer("Не задан адрес сервера в файле конфигурации!");
				 return;
				 //TODO windows from write ip adress
			 }

			 BinaryFormatter ^br = gcnew BinaryFormatter ();
			 TcpClient ^myclient = gcnew TcpClient (CharToSystemString(buf),7000);
			 NetworkStream ^myns = myclient->GetStream ();
			 br->Serialize (myns,file_name);

			 BinaryWriter ^mysw = gcnew BinaryWriter (myns);
			 mysw->Write(buffer);
			 mysw->Close();

			 myns->Close();
			 myclient->Close();

			 Sleep(1000);
		 }
		 catch (Exception^ exc)
		 {
			 okey--;
			 Write::logWrite(exc->Message,"EXCEPTION","pc");
		 }
		 finally
		 {
			 okey++;

			 if(okey = t_log)
				 Write::logWrite("Логи выгружены успешно!","SYSTEM","pc");
		 }
	}
}

Void Form1::backgroundWorker1_RunWorkerCompleted(System::Object^  sender, System::ComponentModel::RunWorkerCompletedEventArgs^  e)
{
	upload_button->Enabled = true;
}

Void Form1::log_upload_timer_Tick(System::Object^  sender, System::EventArgs^  e)
{
	char buf[17];

	GetPrivateProfileString("SETTINGS", "upload_log","false",buf,sizeof(buf),SystemStringToChar(Environment::CurrentDirectory+"\\config.ini"));

	if(CharToSystemString(buf) == "false")
		return;

	Write::logWrite("Автоматическая выгрузка логов","SYSTEM","pc");
	backgroundWorker1->RunWorkerAsync();
}

Void Form1::queryfive(String^ bar)
{
	MySqlDataReader^ reader = nullptr;

	try
	{
		server1Conn->Open();

		char buf[4];
		GetPrivateProfileString("SETTINGS", "id_pricelist","1",buf,sizeof(buf),SystemStringToChar(Environment::CurrentDirectory+"\\config.ini"));

		cmd = gcnew MySqlCommand("SELECT a.name, b.price \n"
			"FROM trm_in_var C \n"
			"LEFT JOIN trm_in_items A ON A.id=C.item \n"
			"LEFT JOIN trm_in_pricelist_items B ON B.item=c.item \n"
			"WHERE a.id='"+bar+"' \n"
			"AND (b.pricelist_id="+CharToSystemString(buf)+")", server1Conn);

		MySqlDataReader^ reader = cmd->ExecuteReader();

		if(reader->Read())
		{
			price_para->Visible = true;
			item_name_textbox->Text = reader->GetString(0);
			price_para->Text =Convert::ToString(reader->GetInt32(1));
			barcode_text_box->Text = "";
			msg_label->Text = "";
			panel4->Visible = false;

			picture_off();

			action_check(bar);
		}
		else
		{
			barcode_text_box->Text = "";
			set_msg_on_timer("Штрих-код не найден!Обратитесь к продавцу!");
		}

	}
	catch (Exception^ exc)
	{
		Write::logWrite(exc->Message,"EXCEPTION","pc");
		set_msg_on_timer("Exception: " + exc->Message);
	}
	finally
	{
		delete cmd;

		if (reader != nullptr)
			reader->Close();

		if (server1Conn->State == ConnectionState::Open)
			server1Conn->Close();
	}
}

Void Form1::picture_off()
{
	pictureBox1->Visible = false;
	image_on->Enabled = true;
}

Void Form1::image_on_Tick(System::Object^  sender, System::EventArgs^  e)
{
	pictureBox1->Visible = true;
	image_on->Enabled = false;

	upsize();
}

Void Form1::upsize()
{
	int x_max = 1272;
	int y_max = 725;

	int x = 1;
	int y = 1;

	pictureBox1->Size = System::Drawing::Size(x,y);

	while(x < x_max)
	{
		x++;

		if(y < y_max)
			y++;

		pictureBox1->Size = System::Drawing::Size(x,y);
	}
}

Void Form1::CheckVersion()
{
	char buf[5];

	GetPrivateProfileString("SETTINGS", "check_version","true",buf,sizeof(buf),SystemStringToChar(Environment::CurrentDirectory+"\\config.ini"));

	if(CharToSystemString(buf) != "true")
	{
		//disable auto_update_timer
		auto_update_timer->Enabled = false;
		return;
	}

	MySqlDataReader^ reader = nullptr;

	try
	{
		server2Conn->Open();

		cmd = gcnew MySqlCommand("SELECT parameter FROM `version` WHERE `name` = 'PriceChecker'", server2Conn);

		MySqlDataReader^ reader = cmd->ExecuteReader();

		if(reader->Read())
		{
			if(version == reader->GetUInt32(0))
			{
				Write::logWrite("Версия проверена успешно.Version = "+ version,"VERSION","pc");
				return;
			}
			else if (version > reader->GetUInt32(0) )
			{
				Write::logWrite("NOT BAD!Версия клиента выше,чем на сервере.Версия клиента:" + version + ".Версия на сервере:"+ reader->GetUInt32(0),"NOTBAD","pc");
			}
			else
			{
				Write::logWrite("Приложение требует обновления.Версия приложения:" + version + ".Версия на сервере: " + (reader->GetUInt32(0)),"VERSION","pc");

				int status = GetPrivateProfileInt("SETTINGS", "status",0,SystemStringToChar(Environment::CurrentDirectory+"\\config.ini"));

				Write::logWrite("Статус последнего обновления:" + status,"STATUS","pc");

				if( status = 1)
				{
					Write::logWrite("Запускаем утитилиту обновления","START","pc");
					System::Diagnostics::Process::Start("Update.exe");

					Write::logWrite("Закрываем Приложение для обновления","EXIT","pc");
					Application::Exit();
				}
				else
				{
					Write::logWrite("Последнее обновления было неудачным,обратитесь к системному администратору","EXCEPTION","pc");
				}
			}
		}
		else
		{
			Write::logWrite("Не удалось проверить версию!","EXCEPTION","pc");
		}
	}
	catch (Exception^ exc)
	{
		Write::logWrite(exc->Message,"EXCEPTION","pc");
		set_msg_on_timer("Exception: " + exc->Message);
	}
	finally
	{
		delete cmd;

		if (reader != nullptr)
			reader->Close();

		if (server2Conn->State == ConnectionState::Open)
			server2Conn->Close();
	}
}

Void Form1::Form1_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e)
{
	Write::logWrite("Терминал остановлен!","SYSTEM","pc");
};