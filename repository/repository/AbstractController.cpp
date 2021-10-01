#include "AbstractController.hpp"

using namespace dsrepository::repository;

AbstractController::AbstractController(QObject* parent)
: QObject(parent)
{
}

AbstractController::~AbstractController()
{
}

QString AbstractController::lastError() const
{
    return lastError_;
}
