
#include "printSetupData.h"

#define Inherited QDialog

#include <qbuttongroup.h>
#include <qgroupbox.h>
#include <qpushbutton.h>
#include <kapp.h>
#include <klocale.h>

printSetupData::printSetupData
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name, TRUE )
{
	QPushButton* tmpQPushButton;
	tmpQPushButton = new QPushButton( this, "PushButton_4" );
	tmpQPushButton->setGeometry( 290, 300, 100, 30 );
	connect( tmpQPushButton, SIGNAL(clicked()), SLOT(reject()) );
	tmpQPushButton->setText( i18n( "Cancel" ) );
	tmpQPushButton->setAutoRepeat( FALSE );
	tmpQPushButton->setAutoResize( FALSE );

	QGroupBox* tmpQGroupBox;
	tmpQGroupBox = new QGroupBox( this, "GroupBox_1" );
	tmpQGroupBox->setGeometry( 150, 10, 250, 240 );
	tmpQGroupBox->setFrameStyle( 49 );
	tmpQGroupBox->setTitle( i18n( "Printers" ) );
	tmpQGroupBox->setAlignment( 1 );

	psnup = new QRadioButton( this, "RadioButton_8" );
	psnup->setGeometry( 30, 170, 100, 30 );
	psnup->setText( i18n( "Use ps&nup" ) );
	psnup->setAutoRepeat( FALSE );
	psnup->setAutoResize( FALSE );

	mpage = new QRadioButton( this, "RadioButton_9" );
	mpage->setGeometry( 30, 200, 100, 30 );
	mpage->setText( i18n( "Use &mpage" ) );
	mpage->setAutoRepeat( FALSE );
	mpage->setAutoResize( FALSE );

	printers = new QListBox( this, "ListBox_1" );
	printers->setGeometry( 170, 30, 210, 120 );
	printers->setFrameStyle( 51 );
	printers->setLineWidth( 2 );

	newPrinter = new QLineEdit( this, "LineEdit_4" );
	newPrinter->setGeometry( 170, 160, 210, 30 );
	connect( newPrinter, SIGNAL(returnPressed()), SLOT(addPrinter()) );
	newPrinter->setText( "" );
	newPrinter->setMaxLength( 32767 );
	newPrinter->setEchoMode( QLineEdit::Normal );
	newPrinter->setFrame( TRUE );

	tmpQPushButton = new QPushButton( this, "PushButton_5" );
	tmpQPushButton->setGeometry( 170, 200, 100, 30 );
	connect( tmpQPushButton, SIGNAL(clicked()), SLOT(addPrinter()) );
	tmpQPushButton->setText( i18n( "&Add" ) );
	tmpQPushButton->setAutoRepeat( FALSE );
	tmpQPushButton->setAutoResize( FALSE );

	tmpQPushButton = new QPushButton( this, "PushButton_6" );
	tmpQPushButton->setGeometry( 280, 200, 100, 30 );
	connect( tmpQPushButton, SIGNAL(clicked()), SLOT(removePrinter()) );
	tmpQPushButton->setText( i18n( "&Remove" ) );
	tmpQPushButton->setAutoRepeat( FALSE );
	tmpQPushButton->setAutoResize( FALSE );

	okButton = new QPushButton( this, "PushButton_7" );
	okButton->setGeometry( 170, 300, 100, 30 );
	connect( okButton, SIGNAL(clicked()), SLOT(okPressed()) );
	okButton->setText( i18n( "OK" ) );
	okButton->setAutoRepeat( FALSE );
	okButton->setAutoResize( FALSE );

	spoolerCommand = new QLineEdit( this, "LineEdit_2" );
	spoolerCommand->setGeometry( 150, 260, 250, 30 );
	spoolerCommand->setText( "" );
	spoolerCommand->setMaxLength( 32767 );
	spoolerCommand->setEchoMode( QLineEdit::Normal );
	spoolerCommand->setFrame( TRUE );

	spoolerLabel = new QLabel( this, "Label_1" );
	spoolerLabel->setGeometry( 10, 260, 130, 30 );
	spoolerLabel->setFrameStyle( 50 );
	spoolerLabel->setText( i18n( "&Spooler command:" ) );
	spoolerLabel->setAlignment( 290 );
	spoolerLabel->setMargin( 4 );

	QButtonGroup* tmpQButtonGroup;
	tmpQButtonGroup = new QButtonGroup( this, "ButtonGroup_5" );
	tmpQButtonGroup->setGeometry( 10, 140, 130, 110 );
	tmpQButtonGroup->setFrameStyle( 49 );
	tmpQButtonGroup->setTitle( i18n( "n-up" ) );
	tmpQButtonGroup->setAlignment( 1 );
	tmpQButtonGroup->insert( psnup );
	tmpQButtonGroup->insert( mpage );
	tmpQButtonGroup->lower();

	resize( 410, 340 );
}


printSetupData::~printSetupData()
{
}
void printSetupData::addPrinter()
{
}
void printSetupData::removePrinter()
{
}
void printSetupData::okPressed()
{
}
#include "printSetupData.moc"
