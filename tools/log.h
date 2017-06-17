#ifndef LOG_H
#define LOG_H

/**
 * @brief 重定向QDebug输出到文件，并附上日期
 * @使用  不动态追加日期setDebugOutput( "log/all.log" );动态追加日期setDebugOutput( "log/%1.log", true );
 * @param rawTargetFilePath_
 * @param argDateFlag_
 */
void setDebugOutput(const QString &rawTargetFilePath_, const bool &argDateFlag_ = false)
{
	static QString rawTargetFilePath;
	static bool argDateFlag;

	rawTargetFilePath = rawTargetFilePath_;
	argDateFlag = argDateFlag_;

	class HelperClass
	{
	public:
		static void messageHandler(QtMsgType type, const QMessageLogContext &, const QString &message_)
		{
			QString message;

			switch ( type )
			{
			case QtDebugMsg:
				{
					message = message_;
					break;
				}
			case QtWarningMsg:
				{
					message.append("Warning: ");
					message.append(message_);
					break;
				}
			case QtCriticalMsg:
				{
					message.append("Critical: ");
					message.append(message_);
					break;
				}
			case QtFatalMsg:
				{
					message.append("Fatal: ");
					message.append(message_);
					break;
				}
			default: { break; }
			}

			QString currentTargetFilePath;

			if ( argDateFlag )
			{
				currentTargetFilePath = rawTargetFilePath.arg( ( ( argDateFlag ) ? ( QDateTime::currentDateTime().toString("yyyy_MM_dd") ) : ( "" ) ) );
			}
			else
			{
				currentTargetFilePath = rawTargetFilePath;
			}

			if ( !QFileInfo::exists( currentTargetFilePath ) )
			{
				QDir().mkpath( QFileInfo( currentTargetFilePath ).path() );
			}

			QFile file( currentTargetFilePath );
			file.open( QIODevice::WriteOnly | QIODevice::Append );

			QTextStream textStream( &file );
			textStream << QDateTime::currentDateTime().toString( "yyyy-MM-dd hh:mm:ss" ) << ": " << message << endl;
		}
	};

	qInstallMessageHandler( HelperClass::messageHandler );
}

#endif // LOG_H
