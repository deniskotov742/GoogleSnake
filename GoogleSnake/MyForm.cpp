#include "MyForm.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]

int main(array<String^>^ args) {
	Application::EnableVisualStyles;
	Application::SetCompatibleTextRenderingDefault(false);

	GoogleSnake::MyForm form;
	Application::Run(% form);
}

struct Vector {
	int X, Y;
};

Vector direction;
Vector fruitPosition;
Vector gameArea;

GoogleSnake::MyForm::MyForm(void) {
	InitializeComponent();
	gameArea.X = 500;
	gameArea.Y = 500;
	firstLaunch = true;
	new_game();
}

void GoogleSnake::MyForm::GeneratePositionFruit() {
	Random^ rand = gcnew Random();
	fruitPosition.X = rand->Next(50, gameArea.X - 70);
	fruitPosition.Y = rand->Next(130, gameArea.Y - 70);

	for (int i = 0; i < score; i++) {
		if (fruitPosition.X == Snake[i]->Location.X &&
			fruitPosition.Y == Snake[i]->Location.Y)
			GeneratePositionFruit();
	}
	int tempX = fruitPosition.X % step;
	fruitPosition.X -= tempX;

	int tempY = fruitPosition.Y % step;
	fruitPosition.Y -= tempY;

	fruit->Location = Point(fruitPosition.X, fruitPosition.Y);

	this->Controls->Add(fruit);
}

void GoogleSnake::MyForm::Eating() {
	if (Snake[0]->Location.X == fruitPosition.X && Snake[0]->Location.Y == fruitPosition.Y) {
		labelScore->Text = "Счет: " + ++score;

		Snake[score] = gcnew PictureBox();
		Snake[score]->Location = Point(Snake[score - 1]->Location.X + step * direction.X,
			Snake[score - 1]->Location.Y - step * direction.Y);
		Snake[score]->BackColor = Color::Red;
		Snake[score]->Width = step;
		Snake[score]->Height = step;
		this->Controls->Add(Snake[score]);

		GeneratePositionFruit();
	}
}

void GoogleSnake::MyForm::Movement() {
	for (int i = score; i >= 1; i--) {
		Snake[i]->Location = Snake[i - 1]->Location;
	}
	Snake[0]->Location = Point(Snake[0]->Location.X + direction.X * step, 
		Snake[0]->Location.Y + direction.Y * step);
}

void GoogleSnake::MyForm::EatYourself() {
	for (int i = 1; i < score; i++) {
		if (Snake[0]->Location == Snake[i]->Location) {
			game_over();
		}
	}
}

void GoogleSnake::MyForm::game_over() {
	play = true;
	death = true;
	labelGameOver->Visible = true;
	labelGameOver2->Visible = true;
}

void GoogleSnake::MyForm::new_game() {
	if (!firstLaunch) {
		this->Controls->Remove(fruit);

		for (int i = 0; i <= score; i++) {
			this->Controls->Remove(Snake[i]);
		}

		score = 0;
	}
	else {
		firstLaunch = false;
	}

	Snake = gcnew array <PictureBox^, 1>(400);
	Snake[0] = gcnew PictureBox();
	Snake[0]->Location = Point(200, 200);
	Snake[0]->BackColor = Color::Green;
	Snake[0]->Width = step;
	Snake[0]->Height = step;

	score = 0;
	this->Controls->Add(Snake[0]);

	fruit = gcnew PictureBox();
	fruit->BackColor = Color::Red;
	fruit->Width = step;
	fruit->Height = step;
	GeneratePositionFruit();

	timer1->Interval = updateInterval;
	timer1->Start();

	direction.X = 1;
	direction.Y = 0;

	play = true;
	death = false;

	labelScore->Text = "Счет: 0";

	labelGameOver->Visible = false;
	labelGameOver2->Visible = false;
	groupBox2->Visible = false;
}

void GoogleSnake::MyForm::checkBorders() {
	if (Snake[0]->Location.X >= pictureBox3->Location.X || Snake[0]->
		Location.X <= pictureBox1->Location.X) {
		game_over();
	}
	if (Snake[0]->Location.Y <= pictureBox2->Location.Y || Snake[0]->
		Location.Y >= pictureBox4->Location.Y) {
		game_over();
	}
	label2->Text = "координаты змейки: (" + Convert::ToString(Snake[0]->
		Location.X) +
		", " + Convert::ToString(Snake[0]->Location.Y) + ")";
}

System::Void GoogleSnake::MyForm::новаяИграToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	new_game();

	return System::Void();
}

System::Void GoogleSnake::MyForm::паузапродолжитьToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (play) {
		play = false;
	}
	else {
		play = true;
		timer1->Start();
	}
	return System::Void();
}

System::Void GoogleSnake::MyForm::настройкиToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (groupBox2->Visible == false) {
		play = false;

		button1->Enabled = true;
		numericUpDown1->Enabled = true;
		groupBox2->Visible = true;
	}
	else {
		play = true;
		timer1->Start();

		button1->Enabled = false;
		numericUpDown1->Enabled = false;
		groupBox2->Visible = false;
	}
}

System::Void GoogleSnake::MyForm::выходToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	Application::Exit();
}

System::Void GoogleSnake::MyForm::button1_Click(System::Object^ sender, System::EventArgs^ e)
{
	updateInterval = Convert::ToSingle(numericUpDown1->Value);
	timer1->Interval = updateInterval;

	button1->Enabled = false;
	numericUpDown1->Enabled = false;
	groupBox2->Visible = false;

	play = true;
	timer1->Start();

	return System::Void();
}

System::Void GoogleSnake::MyForm::MyForm_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e)
{
	if (e->KeyCode.ToString() == "Right" || e->KeyCode.ToString() == "D") {
		direction.X = 1;
		direction.Y = 0;
	}
	else if (e->KeyCode.ToString() == "Left" || e->KeyCode.ToString() == "A") {
		direction.X = -1;
		direction.Y = 0;
	}
	else if (e->KeyCode.ToString() == "Up" || e->KeyCode.ToString() == "W") {
		direction.X = 0;
		direction.Y = -1;
	}
	else if (e->KeyCode.ToString() == "Down" || e->KeyCode.ToString() == "S") {
		direction.X = 0;
		direction.Y = 1;
	}
	return System::Void();
}

System::Void GoogleSnake::MyForm::информацияОбИгреToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	MessageBox::Show("Правила игры:\n"
		"1. для управления использовать стрелки\n"
		"2. ешьте фрукты для набора счета и увеличения змейки\n"
		"3. нельзя есть себя и врезаться в стенки\n"
		"4. и т.д.", "правила игры!");
	return System::Void();
}

System::Void GoogleSnake::MyForm::timer1_Tick(System::Object^ sender, System::EventArgs^ e)
{
	if (!death && play) {
		Movement();
		Eating();
		EatYourself();
		checkBorders();
	}
	else if (death && play) {
		timer1->Stop();
		MessageBox::Show("Игра окончена!", "Внимание!");
	}
	else if (!play && !death) {
		timer1->Stop();
		MessageBox::Show("Игра приостоновлена!", "Внимание!");
	}
}
