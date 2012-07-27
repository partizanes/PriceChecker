#pragma once

namespace PriceChecker {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace MySql::Data::MySqlClient;

	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
		}

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
    private: System::Windows::Forms::TextBox^  barcode_text_box;
    private: System::Windows::Forms::TextBox^  item_name_textbox;
    private: System::Windows::Forms::Label^  price_textbox;

	
    private: System::Windows::Forms::Label^  price_para;
    private: System::Windows::Forms::Label^  balance_para;
    private: System::Windows::Forms::Label^  old_price_para;

    private: System::Windows::Forms::Label^  barcode_label;
    private: System::Windows::Forms::Label^  action_label;
    private: System::Windows::Forms::Label^  balance_label;
    private: System::Windows::Forms::Label^  old_price_label;
    private: System::Windows::Forms::Label^  name_label;

    private: System::Windows::Forms::PictureBox^  pictureBox1;
    private: System::Windows::Forms::Label^  label1;

    private: System::Windows::Forms::Timer^  timer1;
    private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::Panel^  panel2;
	private: System::Windows::Forms::Panel^  panel3;
	private: System::Windows::Forms::Panel^  panel4;
	public: System::Windows::Forms::Label^  msg_label;
	private: 

	private: System::Windows::Forms::Timer^  msg_clear;
	private: System::Windows::Forms::Label^  total_para;

	private: System::Windows::Forms::Label^  total_label;
	private: System::Windows::Forms::Label^  weight_para;
	private: System::Windows::Forms::Label^  weight_label;
	private: System::Windows::Forms::Timer^  weight_clr;
	private: System::Windows::Forms::Timer^  pass_timer;
	private: System::Windows::Forms::Panel^  stg_panel;
	private: System::Windows::Forms::Label^  dir_exist_para;
	private: System::Windows::Forms::Label^  dir_exist;
	private: System::Windows::Forms::Label^  mysql_check_para;
	private: System::Windows::Forms::Label^  mysql_check;
	private: System::Windows::Forms::Button^  close_menu;
	private: System::Windows::Forms::Button^  test_button;

	private: 









	private: System::ComponentModel::IContainer^  components;

	protected: 

	private:


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Обязательный метод для поддержки конструктора - не изменяйте
		/// содержимое данного метода при помощи редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			this->barcode_text_box = (gcnew System::Windows::Forms::TextBox());
			this->barcode_label = (gcnew System::Windows::Forms::Label());
			this->item_name_textbox = (gcnew System::Windows::Forms::TextBox());
			this->name_label = (gcnew System::Windows::Forms::Label());
			this->price_textbox = (gcnew System::Windows::Forms::Label());
			this->price_para = (gcnew System::Windows::Forms::Label());
			this->balance_label = (gcnew System::Windows::Forms::Label());
			this->balance_para = (gcnew System::Windows::Forms::Label());
			this->action_label = (gcnew System::Windows::Forms::Label());
			this->old_price_label = (gcnew System::Windows::Forms::Label());
			this->old_price_para = (gcnew System::Windows::Forms::Label());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->panel3 = (gcnew System::Windows::Forms::Panel());
			this->total_para = (gcnew System::Windows::Forms::Label());
			this->total_label = (gcnew System::Windows::Forms::Label());
			this->weight_para = (gcnew System::Windows::Forms::Label());
			this->weight_label = (gcnew System::Windows::Forms::Label());
			this->panel4 = (gcnew System::Windows::Forms::Panel());
			this->msg_label = (gcnew System::Windows::Forms::Label());
			this->msg_clear = (gcnew System::Windows::Forms::Timer(this->components));
			this->weight_clr = (gcnew System::Windows::Forms::Timer(this->components));
			this->pass_timer = (gcnew System::Windows::Forms::Timer(this->components));
			this->stg_panel = (gcnew System::Windows::Forms::Panel());
			this->test_button = (gcnew System::Windows::Forms::Button());
			this->close_menu = (gcnew System::Windows::Forms::Button());
			this->mysql_check_para = (gcnew System::Windows::Forms::Label());
			this->mysql_check = (gcnew System::Windows::Forms::Label());
			this->dir_exist_para = (gcnew System::Windows::Forms::Label());
			this->dir_exist = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
			this->panel1->SuspendLayout();
			this->panel2->SuspendLayout();
			this->panel3->SuspendLayout();
			this->panel4->SuspendLayout();
			this->stg_panel->SuspendLayout();
			this->SuspendLayout();
			// 
			// barcode_text_box
			// 
			this->barcode_text_box->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), 
				static_cast<System::Int32>(static_cast<System::Byte>(192)), static_cast<System::Int32>(static_cast<System::Byte>(128)));
			this->barcode_text_box->Cursor = System::Windows::Forms::Cursors::Arrow;
			resources->ApplyResources(this->barcode_text_box, L"barcode_text_box");
			this->barcode_text_box->Name = L"barcode_text_box";
			this->barcode_text_box->TabStop = false;
			this->barcode_text_box->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Form1::barcode_text_box_KeyDown);
			this->barcode_text_box->Leave += gcnew System::EventHandler(this, &Form1::barcode_text_box_Leave);
			// 
			// barcode_label
			// 
			resources->ApplyResources(this->barcode_label, L"barcode_label");
			this->barcode_label->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->barcode_label->ForeColor = System::Drawing::Color::DarkRed;
			this->barcode_label->Name = L"barcode_label";
			// 
			// item_name_textbox
			// 
			this->item_name_textbox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), 
				static_cast<System::Int32>(static_cast<System::Byte>(192)), static_cast<System::Int32>(static_cast<System::Byte>(128)));
			this->item_name_textbox->Cursor = System::Windows::Forms::Cursors::Arrow;
			resources->ApplyResources(this->item_name_textbox, L"item_name_textbox");
			this->item_name_textbox->ForeColor = System::Drawing::Color::Black;
			this->item_name_textbox->Name = L"item_name_textbox";
			this->item_name_textbox->Click += gcnew System::EventHandler(this, &Form1::item_name_textbox_Click);
			this->item_name_textbox->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::item_name_textbox_MouseClick);
			this->item_name_textbox->DoubleClick += gcnew System::EventHandler(this, &Form1::item_name_textbox_DoubleClick);
			this->item_name_textbox->Enter += gcnew System::EventHandler(this, &Form1::item_name_textbox_Enter);
			this->item_name_textbox->MouseDoubleClick += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::item_name_textbox_MouseDoubleClick);
			// 
			// name_label
			// 
			resources->ApplyResources(this->name_label, L"name_label");
			this->name_label->ForeColor = System::Drawing::Color::DarkRed;
			this->name_label->Name = L"name_label";
			// 
			// price_textbox
			// 
			resources->ApplyResources(this->price_textbox, L"price_textbox");
			this->price_textbox->ForeColor = System::Drawing::Color::DarkRed;
			this->price_textbox->Name = L"price_textbox";
			// 
			// price_para
			// 
			resources->ApplyResources(this->price_para, L"price_para");
			this->price_para->ForeColor = System::Drawing::Color::Red;
			this->price_para->Name = L"price_para";
			// 
			// balance_label
			// 
			resources->ApplyResources(this->balance_label, L"balance_label");
			this->balance_label->ForeColor = System::Drawing::Color::DarkRed;
			this->balance_label->Name = L"balance_label";
			// 
			// balance_para
			// 
			resources->ApplyResources(this->balance_para, L"balance_para");
			this->balance_para->ForeColor = System::Drawing::Color::DarkRed;
			this->balance_para->Name = L"balance_para";
			// 
			// action_label
			// 
			resources->ApplyResources(this->action_label, L"action_label");
			this->action_label->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(192)), 
				static_cast<System::Int32>(static_cast<System::Byte>(0)));
			this->action_label->Name = L"action_label";
			// 
			// old_price_label
			// 
			resources->ApplyResources(this->old_price_label, L"old_price_label");
			this->old_price_label->ForeColor = System::Drawing::SystemColors::GradientActiveCaption;
			this->old_price_label->Name = L"old_price_label";
			// 
			// old_price_para
			// 
			resources->ApplyResources(this->old_price_para, L"old_price_para");
			this->old_price_para->ForeColor = System::Drawing::SystemColors::GradientInactiveCaption;
			this->old_price_para->Name = L"old_price_para";
			// 
			// pictureBox1
			// 
			this->pictureBox1->BackColor = System::Drawing::Color::Black;
			resources->ApplyResources(this->pictureBox1, L"pictureBox1");
			this->pictureBox1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->TabStop = false;
			// 
			// label1
			// 
			resources->ApplyResources(this->label1, L"label1");
			this->label1->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->label1->ForeColor = System::Drawing::Color::Crimson;
			this->label1->Name = L"label1";
			// 
			// timer1
			// 
			this->timer1->Interval = 5000;
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// panel1
			// 
			this->panel1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(192)), 
				static_cast<System::Int32>(static_cast<System::Byte>(128)));
			this->panel1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->panel1->Controls->Add(this->label1);
			resources->ApplyResources(this->panel1, L"panel1");
			this->panel1->Name = L"panel1";
			// 
			// panel2
			// 
			this->panel2->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(192)), 
				static_cast<System::Int32>(static_cast<System::Byte>(128)));
			this->panel2->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->panel2->Controls->Add(this->barcode_label);
			this->panel2->Controls->Add(this->barcode_text_box);
			this->panel2->Controls->Add(this->name_label);
			this->panel2->Controls->Add(this->item_name_textbox);
			resources->ApplyResources(this->panel2, L"panel2");
			this->panel2->Name = L"panel2";
			// 
			// panel3
			// 
			this->panel3->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(192)), 
				static_cast<System::Int32>(static_cast<System::Byte>(128)));
			this->panel3->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->panel3->Controls->Add(this->total_para);
			this->panel3->Controls->Add(this->total_label);
			this->panel3->Controls->Add(this->weight_para);
			this->panel3->Controls->Add(this->weight_label);
			this->panel3->Controls->Add(this->balance_para);
			this->panel3->Controls->Add(this->old_price_label);
			this->panel3->Controls->Add(this->balance_label);
			this->panel3->Controls->Add(this->old_price_para);
			this->panel3->Controls->Add(this->price_textbox);
			this->panel3->Controls->Add(this->price_para);
			resources->ApplyResources(this->panel3, L"panel3");
			this->panel3->Name = L"panel3";
			// 
			// total_para
			// 
			resources->ApplyResources(this->total_para, L"total_para");
			this->total_para->ForeColor = System::Drawing::Color::Red;
			this->total_para->Name = L"total_para";
			// 
			// total_label
			// 
			resources->ApplyResources(this->total_label, L"total_label");
			this->total_label->Name = L"total_label";
			// 
			// weight_para
			// 
			resources->ApplyResources(this->weight_para, L"weight_para");
			this->weight_para->ForeColor = System::Drawing::Color::Red;
			this->weight_para->Name = L"weight_para";
			// 
			// weight_label
			// 
			resources->ApplyResources(this->weight_label, L"weight_label");
			this->weight_label->ForeColor = System::Drawing::Color::DarkRed;
			this->weight_label->Name = L"weight_label";
			// 
			// panel4
			// 
			this->panel4->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(192)), 
				static_cast<System::Int32>(static_cast<System::Byte>(128)));
			this->panel4->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->panel4->Controls->Add(this->action_label);
			this->panel4->Controls->Add(this->msg_label);
			resources->ApplyResources(this->panel4, L"panel4");
			this->panel4->Name = L"panel4";
			// 
			// msg_label
			// 
			resources->ApplyResources(this->msg_label, L"msg_label");
			this->msg_label->ForeColor = System::Drawing::Color::Black;
			this->msg_label->Name = L"msg_label";
			// 
			// msg_clear
			// 
			this->msg_clear->Interval = 8000;
			this->msg_clear->Tick += gcnew System::EventHandler(this, &Form1::msg_clear_Tick);
			// 
			// weight_clr
			// 
			this->weight_clr->Interval = 8000;
			this->weight_clr->Tick += gcnew System::EventHandler(this, &Form1::weight_clr_Tick);
			// 
			// pass_timer
			// 
			this->pass_timer->Interval = 15000;
			this->pass_timer->Tick += gcnew System::EventHandler(this, &Form1::pass_timer_Tick);
			// 
			// stg_panel
			// 
			this->stg_panel->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(192)), 
				static_cast<System::Int32>(static_cast<System::Byte>(128)));
			this->stg_panel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->stg_panel->Controls->Add(this->test_button);
			this->stg_panel->Controls->Add(this->close_menu);
			this->stg_panel->Controls->Add(this->mysql_check_para);
			this->stg_panel->Controls->Add(this->mysql_check);
			this->stg_panel->Controls->Add(this->dir_exist_para);
			this->stg_panel->Controls->Add(this->dir_exist);
			resources->ApplyResources(this->stg_panel, L"stg_panel");
			this->stg_panel->Name = L"stg_panel";
			// 
			// test_button
			// 
			resources->ApplyResources(this->test_button, L"test_button");
			this->test_button->Name = L"test_button";
			this->test_button->UseVisualStyleBackColor = true;
			this->test_button->Click += gcnew System::EventHandler(this, &Form1::test_button_Click);
			// 
			// close_menu
			// 
			resources->ApplyResources(this->close_menu, L"close_menu");
			this->close_menu->Name = L"close_menu";
			this->close_menu->UseVisualStyleBackColor = true;
			this->close_menu->Click += gcnew System::EventHandler(this, &Form1::close_menu_Click);
			// 
			// mysql_check_para
			// 
			resources->ApplyResources(this->mysql_check_para, L"mysql_check_para");
			this->mysql_check_para->Name = L"mysql_check_para";
			// 
			// mysql_check
			// 
			resources->ApplyResources(this->mysql_check, L"mysql_check");
			this->mysql_check->ForeColor = System::Drawing::Color::MediumBlue;
			this->mysql_check->Name = L"mysql_check";
			// 
			// dir_exist_para
			// 
			resources->ApplyResources(this->dir_exist_para, L"dir_exist_para");
			this->dir_exist_para->Name = L"dir_exist_para";
			// 
			// dir_exist
			// 
			resources->ApplyResources(this->dir_exist, L"dir_exist");
			this->dir_exist->ForeColor = System::Drawing::Color::Blue;
			this->dir_exist->Name = L"dir_exist";
			// 
			// Form1
			// 
			resources->ApplyResources(this, L"$this");
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(192)), 
				static_cast<System::Int32>(static_cast<System::Byte>(128)));
			this->ControlBox = false;
			this->Controls->Add(this->stg_panel);
			this->Controls->Add(this->panel4);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->panel2);
			this->Controls->Add(this->panel3);
			this->DoubleBuffered = true;
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"Form1";
			this->ShowIcon = false;
			this->ShowInTaskbar = false;
			this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
			this->panel1->ResumeLayout(false);
			this->panel1->PerformLayout();
			this->panel2->ResumeLayout(false);
			this->panel2->PerformLayout();
			this->panel3->ResumeLayout(false);
			this->panel3->PerformLayout();
			this->panel4->ResumeLayout(false);
			this->panel4->PerformLayout();
			this->stg_panel->ResumeLayout(false);
			this->stg_panel->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion

private: MySqlConnection^	conn;
private: MySqlCommand^	cmd;
	
private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e);
private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e);

private: System::Void item_name_textbox_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void item_name_textbox_DoubleClick(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void item_name_textbox_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
		 }
private: System::Void item_name_textbox_MouseDoubleClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
		 }
private: System::Void item_name_textbox_Enter(System::Object^  sender, System::EventArgs^  e) {
			 this->barcode_text_box->Focus();
		 }

private: System::Void barcode_text_box_Leave(System::Object^  sender, System::EventArgs^  e) {
			 this->barcode_text_box->Focus();
		 }
private: System::Boolean ean13_validate(int barcode[]);
private: System::Boolean ean8_validate(int barcode[]);
private: System::Void log_write(String^ str,String^ reason);
private: System::Void msg_clear_Tick(System::Object^  sender, System::EventArgs^  e) {
			 msg_clear->Enabled = false;
			 msg_label->Text = "";
			 msg_label->Visible = false;
			 panel4->Visible = false;
		 }
private: System::Void Form1::query(String^ bar);
private: System::Void Form1::set_msg_on_timer(String^ text);
private: System::Void weight_clr_Tick(System::Object^  sender, System::EventArgs^  e) {
			 weight_para->Visible = false;
			 weight_label->Visible = false;
			 total_label->Visible = false;
			 total_para->Visible = false;
			 weight_clr->Enabled = false;
			 panel4->Visible = false;
		 }
private: System::Void barcode_text_box_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e);
private: System::Void pass_timer_Tick(System::Object^  sender, System::EventArgs^  e);
private: System::Void close_menu_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void diag_system();
private: System::Boolean mysqlcheck();
private: System::Void test_button_Click(System::Object^  sender, System::EventArgs^  e);
};
}

