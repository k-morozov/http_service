//
// Created by focus on 04.12.2021.
//
#include "pch.h"

#include "logger.h"

#include "LoggerImpl.h"


Logger::Logger(std::string const& channel_name, std::string const& tag)
{
    lg_ = std::make_unique<LoggerImpl>(channel_name, tag);
    if (!lg_)
        throw std::bad_alloc();
    lg_->init();
}

Logger::~Logger() = default;

void Logger::write(sdk::Severity level, std::string const& message)
{
    lg_->write(level, message);
}

void Logger::trace(const std::string &message) {
    write(sdk::Severity::Trace, message);
}

void Logger::debug(std::string const& message)
{
    write(sdk::Severity::Debug, message);
}

void Logger::info(std::string const& message)
{
    write(sdk::Severity::Info, message);
}

void Logger::warning(const std::string &message) {
    write(sdk::Severity::Warning, message);
}

void Logger::error(std::string const& message)
{
    write(sdk::Severity::Error, message);
}

void Logger::fatal(const std::string &message) {
    write(sdk::Severity::Fatal, message);
}