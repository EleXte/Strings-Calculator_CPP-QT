#include <Calc_widget.h>

AE_CalcWidget::AE_CalcWidget(QWidget *parent)
    : QWidget(parent)
    ,   ui(new Ui::Form)
{
    ui->setupUi(this);
	/*public CalculatorFrame() {
		setTitle("Calculator");
		CalculatorPanel panel = new CalculatorPanel();
		add(panel);
		pack();
	}*/
/*
	QUiLoader loader;
    QString path = "/calc_form.ui";
	QFile f(path); 
    f.open(QFile::ReadOnly);
    // грузим виджет из UI
    QWidget* newWidget = loader.load( &f, this );
	
    this->setupUi(&newWidget);

	f.close();
*/
}

AE_CalcWidget::~AE_CalcWidget()
{
   delete ui;
}

void AE_CalcWidget::ins_sym()
{

    QPushButton* but = qobject_cast<QPushButton*>(sender());
    ui->lineEdit->insert(but->text());
    //ui->lineEdit->insert(((QPusnButton*)sender())->text());
}

void AE_CalcWidget::on_pb_clear_clicked()
{
    ui->lineEdit->clear();
    if (ui->ch_Box_clean->isChecked() == true) {
        ui->textEdit->clear();
    }
}

void AE_CalcWidget::on_pb_rem_clicked()
{
    ui->lineEdit->backspace();
}

void AE_CalcWidget::on_pb_enter_clicked()
{
    AE_CalcString *Calc = new AE_CalcString;

    QString exp = Calc->getString(ui->lineEdit->text());

    if (ui->ch_Box_oneline->isChecked() == true) {
        ui->textEdit->append(ui->lineEdit->text() + " = " + exp);
    } else {
        ui->textEdit->append(ui->lineEdit->text());
        ui->textEdit->append("= " + exp);
    }

    if (ui->ch_Box_1->isChecked() == false) {
        ui->lineEdit->clear();
    }
}
