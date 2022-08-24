/*#pragma once
#include <QMap>
#include <QList>
#include <QFile>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonvalue.h>

class Notes
{
private:
	QList<Note> listNotes;
public:
	Notes() {
		listNotes = QList<Note>();
	}

	// give me a json
	Notes(QString json_url) {
		listNotes = QList<Note>();
		LoadJson(json_url);
	}

	void LoadJson(QString json_url) {
		ParseJsonToNote(json_url);
	}

	void ParseJsonToNote(QString json_url) {
		// read file
		QString json ;
		QFile file;
		file.setFileName("json_url");
		file.open(QIODevice::ReadOnly | QIODevice::Text);
		json = file.readAll();
		file.close();
		QJsonDocument note = QJsonDocument::fromJson(json.toUtf8());
		QJsonObject a = note.object();
		Note nTemp = Note();
		for (int i = 1; i < a.count(); i++) {
			QJsonValue value = a.value(QString("i"));
			QJsonObject item = value.toObject();
			QJsonValue time = item["time"];
			QJsonValue content = item["content"];
			nTemp.ID = i;
			/*nTemp.Properties["time"] = time;
			nTemp.Properties["Content"] = content;*/
//		}
		
//	}

	/*void AddNote(Note item) {
		listNotes.append(item);
	}

	void AddNote(QString time, QString content) {
		Note item;
		item.ID = 0;
		item.Properties["time"] = time;
		item.Properties["content"] = content;
		listNotes.append(item);
	}

	void SaveNoteToFile(QString to_url) {
		// save file
	}

	Note GetNote(int index) {
		Note result;

		if (index < listNotes.count()) {
			result = Note(listNotes[index]);
			listNotes.removeAt(index);
		}

		return result;
	}

	Note Retrieve(int index) {
		Note result;
		if (index < listNotes.count()) {
			result = Note(listNotes[index]);
		}

		return result;
	}

	int Count() {
		return this->listNotes.count();
	}

	Note NoteAt(int index) {
		Note result;
		if (index < listNotes.count()) {
			result = listNotes[index];
		}

		return result;
	}
};



class Note {

public:
	int ID;
	QMap<QString, QString> Properties;

	Note() {
		ID = -1;
		Properties = QMap<QString,QString>();
	}

	Note (Note &n) {
		ID = n.ID;
		Properties = n.Properties;
	}
};*/
