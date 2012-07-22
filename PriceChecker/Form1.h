#pragma once

namespace PriceChecker {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Сводка для Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//
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
	private: System::ComponentModel::IContainer^  components;

	protected: 


	private:
		/// <summary>
		/// Требуется переменная конструктора.
		/// </summary>


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
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
			this->panel1->SuspendLayout();
			this->SuspendLayout();
			// 
			// barcode_text_box
			// 
			resources->ApplyResources(this->barcode_text_box, L"barcode_text_box");
			this->barcode_text_box->Name = L"barcode_text_box";
			this->barcode_text_box->TabStop = false;
			this->barcode_text_box->TextChanged += gcnew System::EventHandler(this, &Form1::barcode_text_box_TextChanged);
			this->barcode_text_box->Leave += gcnew System::EventHandler(this, &Form1::barcode_text_box_Leave);
			// 
			// barcode_label
			// 
			resources->ApplyResources(this->barcode_label, L"barcode_label");
			this->barcode_label->Name = L"barcode_label";
			// 
			// item_name_textbox
			// 
			resources->ApplyResources(this->item_name_textbox, L"item_name_textbox");
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
			this->name_label->Name = L"name_label";
			// 
			// price_textbox
			// 
			resources->ApplyResources(this->price_textbox, L"price_textbox");
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
			this->action_label->ForeColor = System::Drawing::Color::Blue;
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
			this->label1->ForeColor = System::Drawing::Color::Crimson;
			this->label1->Name = L"label1";
			// 
			// timer1
			// 
			this->timer1->Interval = 3000;
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// panel1
			// 
			this->panel1->BackColor = System::Drawing::Color::Silver;
			this->panel1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->panel1->Controls->Add(this->label1);
			resources->ApplyResources(this->panel1, L"panel1");
			this->panel1->Name = L"panel1";
			// 
			// Form1
			// 
			resources->ApplyResources(this, L"$this");
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::LemonChiffon;
			this->ControlBox = false;
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->old_price_label);
			this->Controls->Add(this->action_label);
			this->Controls->Add(this->balance_para);
			this->Controls->Add(this->balance_label);
			this->Controls->Add(this->price_para);
			this->Controls->Add(this->price_textbox);
			this->Controls->Add(this->name_label);
			this->Controls->Add(this->item_name_textbox);
			this->Controls->Add(this->barcode_label);
			this->Controls->Add(this->barcode_text_box);
			this->Controls->Add(this->old_price_para);
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
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	
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
private: System::Void barcode_text_box_TextChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Boolean ean13_validate(int barcode[]);
};
}

