#include "hello_gui.h"
#include "ui_hello_gui.h"

HelloGui::HelloGui(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::HelloGui),
  serial(new QSerialPort(this)),//
  bms_timer(new QTimer(this)),
  delay_timer(new QTimer(this)),
  BMS_ST(false),
  bms_ready(false)
{
  ui->setupUi(this);

//  nh_.reset(new ros::NodeHandle("~"));//reset method is contained in to smart ptr
  nh_.reset(new ros::NodeHandle("~"));

  ros_timer = new QTimer(this);//
  connect(ros_timer,SIGNAL(timeout()),this,SLOT(spinOnce()));//
  ros_timer->start(100);//

  std::string listen_topic;
//  nh_->param<std::string>("listen_topic",listen_topic,"/talker/chatter");
  nh_->param<std::string>("listen_topic",listen_topic,"/battery");
  test_node = nh_->subscribe<std_msgs::String>(listen_topic,1,&HelloGui::smthcallback, this);

  joy_pub = nh_->advertise<sensor_msgs::Joy>("/joy", 10);//



  mod_pub = nh_->advertise<std_msgs::String>("/deepEx/NMT", 10);//
  bms_pub = nh_->advertise<sensor_msgs::BatteryState>("/battery", 10);

  //SERIAL PORT INIT FOR BMS
  serial->setPortName("/dev/ttyUSB0");
  serial->setBaudRate(QSerialPort::Baud115200);
  serial->setDataBits(QSerialPort::Data8);
  serial->setParity(QSerialPort::NoParity);
  serial->setStopBits(QSerialPort::OneStop);
  serial->setFlowControl(QSerialPort::NoFlowControl);


  delay_timer->setSingleShot(true);

  if (serial->open(QIODevice::ReadWrite)) {
      qDebug() << "SERIAL CONNECTION SUCCESS /dev/ttyUSB0";
      connect(bms_timer, &QTimer::timeout, this, &HelloGui::readSerialData);
      bms_timer->start(2000);
  } else {
      qDebug() << "Failed to open serial port /dev/ttyUSB0.";
  }


//  ros::Subscriber test_node;
}

HelloGui::~HelloGui()
{
  delete ui;
  if (serial->isOpen()) {
          serial->close();
      }
}

void HelloGui::spinOnce(){
  if(ros::ok()){
    ros::spinOnce();
  }
  else {
    QApplication::quit();
  }
}

void HelloGui::smthcallback(const std_msgs::String::ConstPtr &msg){
  auto qstring_msg = QString::fromStdString(msg->data.c_str());

//  ui->log_window->setText(qstring_msg);
  ui->log_window->append(qstring_msg);
  ui->log_window->append(qstring_msg);
}




//ui->log_window->append(QString::number(receivedValue));



void HelloGui::on_log_clear_clicked()
{
//    qDebug() << "on_log_clear_clicked :";
    ui->log_window->clear();
}

void HelloGui::on_torque_off_clicked()
{
//    qDebug() << "on_torque_off_clicked :";
    cmd_msg.axes = std::vector<float>{0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    cmd_msg.buttons = std::vector<int>{0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0};  // 5번째 버튼 활성화
    joy_pub.publish(cmd_msg);
    ROS_INFO("Published Joy Message");
}

void HelloGui::on_torque_on_clicked()
{
//    qDebug() << "on_torque_on_clicked :";
    cmd_msg.axes = std::vector<float>{0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    cmd_msg.buttons = std::vector<int>{0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0};  // 5번째 버튼 활성화
    joy_pub.publish(cmd_msg);
    ROS_INFO("Published Joy Message");
}

void HelloGui::on_pdo_mapping_clicked()
{
    qDebug() << "on_pdo_mapping_clicked :";
    mod_msg.data="pdo_mapping";
    mod_pub.publish(mod_msg);
    ROS_INFO("Published mode Message");
}

void HelloGui::on_remote_mode_clicked()
{
    qDebug() << "on_remote_mode_clicked :";
    mod_msg.data="start_remote";
    mod_pub.publish(mod_msg);
    ROS_INFO("Published mode Message");

}

void HelloGui::on_tele_cvt_stateChanged(int state)
{
    if (state==0){
    qDebug() << "tele state 0 : none check";
    }
    if (state==2){
    qDebug() << "tele state1  : check";
    }
}

void HelloGui::on_make_draw_stateChanged(int state)
{
    if (state==0){
    qDebug() << "state 0 :";

    }
    if (state==2){
    qDebug() << "state1  :";
    }
    }




void HelloGui::on_BMS_ON_clicked()
{
    qDebug() << "BMS ON";

    BMS_ST=true;
    ser_message = QByteArray::fromHex("6C");
    rawdata = ser_message.constData();
    serial->write(rawdata,ser_message.size());
    qDebug() << "Sent:" << ser_message;
    if (serial->waitForReadyRead(1000)){
        rcvmsg=serial->readLine();
        qDebug() << "received:" << rcvmsg;
    }
    else{
        qDebug() << "nothing received:" ;
    }
    rcvmsg=serial->readLine();



    ser_message = QByteArray::fromHex("20");
//    serial->write(ser_message);
    rawdata = ser_message.constData();
    serial->write(rawdata,ser_message.size());
    qDebug() << "Sent:" << ser_message;
    if (serial->waitForReadyRead(1000)){
        rcvmsg=serial->readLine();
        qDebug() << "received:" << rcvmsg;
    }
    else{
        qDebug() << "nothing received:" ;
    }
//    rcvmsg=serial->readLine();


    ser_message = QByteArray::fromHex("32");//30+sec
//    serial->write(ser_message);
    rawdata = ser_message.constData();
    serial->write(rawdata,ser_message.size());
    qDebug() << "Sent:" << ser_message;
    if (serial->waitForReadyRead(1000)){
        rcvmsg=serial->readLine();
        qDebug() << "received:" << rcvmsg;
    }
    else{
        qDebug() << "nothing received:" ;
    }
//    rcvmsg=serial->readLine();


    ser_message = QByteArray::fromHex("0A");
//    serial->write(ser_message);
    rawdata = ser_message.constData();
    serial->write(rawdata,ser_message.size());
    qDebug() << "Sent:" << ser_message;
    if (serial->waitForReadyRead(1000)){
        rcvmsg=serial->readLine();
        qDebug() << "received:" << rcvmsg;
    }
    else{
        qDebug() << "nothing received:" ;
    }
//    rcvmsg=serial->readLine();



    ser_message = QByteArray::fromHex("0D");
//    serial->write(ser_message);
    rawdata = ser_message.constData();
    serial->write(rawdata,ser_message.size());
    qDebug() << "Sent:" << ser_message;
    if (serial->waitForReadyRead(1000)){
        rcvmsg=serial->readLine();
        qDebug() << "received:" << rcvmsg;
    }
    else{
        qDebug() << "nothing received:" ;
    }
    rcvmsg=serial->readLine();

//    delay_timer->start(2000);

    bms_ready=true;



}

void HelloGui::on_BMS_OFF_clicked()
{
    qDebug() << "BMS OFF";
    ser_message = QByteArray::fromHex("6C");
    rawdata = ser_message.constData();
    serial->write(rawdata,ser_message.size());
    qDebug() << "Sent:" << ser_message;
    if (serial->waitForReadyRead(1000)){
        rcvmsg=serial->readLine();
        qDebug() << "received:" << rcvmsg;
    }
    else{
        qDebug() << "nothing received:" ;
    }


    ser_message = QByteArray::fromHex("20");
    rawdata = ser_message.constData();
    serial->write(rawdata,ser_message.size());
    qDebug() << "Sent:" << ser_message;
    if (serial->waitForReadyRead(1000)){
        rcvmsg=serial->readLine();
        qDebug() << "received:" << rcvmsg;
    }
    else{
        qDebug() << "nothing received:" ;
    }
    ser_message = QByteArray::fromHex("30");//30+sec
    rawdata = ser_message.constData();
    serial->write(rawdata,ser_message.size());
    qDebug() << "Sent:" << ser_message;
    if (serial->waitForReadyRead(1000)){
        rcvmsg=serial->readLine();
        qDebug() << "received:" << rcvmsg;
    }
    else{
        qDebug() << "nothing received:" ;
    }
    ser_message = QByteArray::fromHex("0A");
    rawdata = ser_message.constData();
    serial->write(rawdata,ser_message.size());
    qDebug() << "Sent:" << ser_message;
    if (serial->waitForReadyRead(1000)){
        rcvmsg=serial->readLine();
        qDebug() << "received:" << rcvmsg;
    }
    else{
        qDebug() << "nothing received:" ;
    }
    ser_message = QByteArray::fromHex("0D");
    rawdata = ser_message.constData();
    serial->write(rawdata,ser_message.size());
    qDebug() << "Sent:" << ser_message;
    if (serial->waitForReadyRead(1000)){
        rcvmsg=serial->readLine();
        qDebug() << "received:" << rcvmsg;
    }
    else{
        qDebug() << "nothing received:" ;
    }
//    delay_timer->start(2000);

    bms_ready=false;
}

void HelloGui::on_BMS_INIT_clicked()
{
    ser_message = QByteArray::fromHex("0A");
    serial->write(ser_message);
//    delay_timer->start(1300);
    ser_message = QByteArray::fromHex("0D");
    serial->write(ser_message);
    delay_timer->start(300);
    QByteArray data = serial->readLine();
    qDebug() << "Received:" << data;
}

void HelloGui::readSerialData() {
    if (BMS_ST && bms_ready && serial->canReadLine()) {
        QByteArray data = serial->readLine().trimmed();
        qDebug() << "Received:" << data;


        QStringList receivedDataList = QString(data).split(" ");
        QList<QString> realDataList;
        for (const QString &data : receivedDataList) {
            if (!data.isEmpty()) {
                realDataList.append(data);
            }
        }

        if (realDataList.size() >= 3) {
                    QString thirdLastItem = realDataList.at(realDataList.size() - 3);
                    qDebug() << "Third last item:" << thirdLastItem;
                    float battery_charge = thirdLastItem.toFloat() / 1000;
                    qDebug()<<battery_charge;
                    bms_msg.percentage = static_cast<float>(battery_charge);
                    bms_pub.publish(bms_msg);

                } else {
                    qDebug() << "Not enough data to extract the third last item.";
                }

    }
}



