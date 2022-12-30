#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "todomodel.h"
#include "todolist.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    // Expose the ToDoModel to qml
    qmlRegisterType<ToDoModel>("ToDo", 1, 0, "ToDoModel");

    // Make QML able to recognize the toDoList
    // Because we don't need to instantiate the ToDoList Object in qml, so here we use qmlRegisterUncreatableType function
    qmlRegisterUncreatableType<ToDoList>("ToDo", 1, 0, "ToDoList",
                                         QStringLiteral("ToDoList should not be created in QML"));

    ToDoList toDoList;

    QQmlApplicationEngine engine;
    // Make toDoList be available to any QML files loaded by the application
    engine.rootContext()->setContextProperty(QStringLiteral("toDoList"), &toDoList);
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
