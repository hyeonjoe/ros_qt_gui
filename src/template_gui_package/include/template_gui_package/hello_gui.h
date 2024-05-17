#ifndef HELLO_GUI_H
#define HELLO_GUI_H

#include <QWidget>
#include <ros/ros.h>
#include <QDebug>
#include <QMainWindow>
#include <qtimer.h>
#include <std_msgs/String.h>
#include <sensor_msgs/Joy.h>
#include <sensor_msgs/BatteryState.h>
//#include <boost/shared_ptr.hpp>
#include <memory>
#include <QtSerialPort/QSerialPort>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class HelloGui; }
QT_END_NAMESPACE

class HelloGui : public QMainWindow
{
    Q_OBJECT

public:
    HelloGui(QWidget *parent = nullptr);
    ~HelloGui();

    void smthcallback(const std_msgs::String::ConstPtr &msg);
    sensor_msgs::Joy cmd_msg;
    std_msgs::String mod_msg;
    sensor_msgs::BatteryState bms_msg;
    QByteArray ser_message;
private slots:
    void spinOnce();//

//    void read_conn_state();

    void on_log_clear_clicked();

    void on_torque_off_clicked();

    void on_torque_on_clicked();

    void on_pdo_mapping_clicked();

    void on_remote_mode_clicked();

    void on_tele_cvt_stateChanged(int state);

    void on_make_draw_stateChanged(int state);

    void on_BMS_ON_clicked();

    void on_BMS_OFF_clicked();

    void on_BMS_INIT_clicked();

    void readSerialData();


private:
    Ui::HelloGui *ui;

    QTimer *ros_timer;
    QTimer *bms_timer;
    QTimer *delay_timer;

    ros::Publisher joy_pub;
    ros::Publisher mod_pub;
    ros::Publisher bms_pub;

    bool BMS_ST;
    bool bms_ready;

//    ros::NodeHandle nh_;
    std::shared_ptr<ros::NodeHandle> nh_;
    ros::Subscriber test_node;
    QSerialPort *serial;

};
#endif // QT_ROS_DEMO2_H
