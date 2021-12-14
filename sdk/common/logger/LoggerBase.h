//
// Created by focus on 06.12.2021.
//

#ifndef ANALYTICS_LOGGERBASE_H
#define ANALYTICS_LOGGERBASE_H

#include <fstream>
#include <mutex>
#include <boost/format.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/sinks/async_frontend.hpp>
#include <boost/log/utility/setup/file.hpp>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/core/null_deleter.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/log/sinks/async_frontend.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/attributes/scoped_attribute.hpp>
#include <boost/log/attributes.hpp>


#include "Severity.h"
#include "Utils.h"


namespace Log {

    namespace impl {

        inline std::once_flag onceFlag;

        namespace logging = boost::log;
        namespace sinks = boost::log::sinks;
        namespace src = boost::log::sources;
        namespace expr = boost::log::expressions;
        namespace attrs = boost::log::attributes;
        namespace keywords = boost::log::keywords;
    }

    namespace name {
        static constexpr auto Channel = "Channel";
        static constexpr auto Tag = "Tag";
        static constexpr auto Class = "Class";
    }

    namespace detail {
        BOOST_LOG_ATTRIBUTE_KEYWORD(line_id, "LineID", unsigned int)
        BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", boost::log::trivial::severity_level)
        BOOST_LOG_ATTRIBUTE_KEYWORD(timeline, "Timeline", impl::attrs::timer::value_type)
        BOOST_LOG_ATTRIBUTE_KEYWORD(channel, name::Channel, std::string)
        BOOST_LOG_ATTRIBUTE_KEYWORD(class_attr, name::Class, std::string)
        BOOST_LOG_ATTRIBUTE_KEYWORD(tag_attr, name::Tag, std::string)
    }

    template<bool mt = false>
    class LoggerBase {
    public:
        using severity_level = boost::log::trivial::severity_level;

        using BaseType = typename std::conditional_t<
                mt,
                impl::src::severity_logger_mt<severity_level>,
                impl::src::severity_logger<severity_level>>;

        template<typename T>
        using shared_ptr = boost::shared_ptr<T>;

        template<typename T>
        using asynchronous_sink = impl::sinks::asynchronous_sink<T>;

        using text_ostream_backend = impl::sinks::text_ostream_backend;

        template<
            class TChannel,
            typename = typename std::enable_if_t<
                !std::is_same_v<typename std::decay_t<TChannel>, LoggerBase>
            >
        >
        explicit LoggerBase(TChannel&& channel_name)  :
                impl_(impl::keywords::severity = severity_level::info)
        {
            using channel_t = typename std::conditional_t<
                    std::is_convertible_v<TChannel&&, std::string>,
                    std::string,
                    typename std::decay_t<TChannel>>;
            impl_.add_attribute(name::Channel, impl::attrs::constant<std::string>(std::forward<channel_t>(channel_name)));

            std::call_once(impl::onceFlag, &LoggerBase<mt>::init, this);
        }

        template<
                class TChannel,
                class TClass,
                typename = typename std::enable_if_t<
                    !std::is_same_v<typename std::decay_t<TChannel>, LoggerBase>
                >
        >
        explicit LoggerBase(TChannel&& channel_name, TClass&& class_name)  :
                LoggerBase(std::forward<TChannel>(channel_name))
        {
            using class_t = typename std::conditional_t<
                    std::is_convertible_v<TClass&&, std::string>,
                    std::string,
                    typename std::decay_t<TClass>>;
            impl_.add_attribute(name::Class, impl::attrs::constant<std::string>(std::forward<class_t>(class_name)));
        }

        template<
                class TChannel,
                class TClass,
                class TTag,
                typename = typename std::enable_if_t<
                    !std::is_same_v<typename std::decay_t<TChannel>, LoggerBase>
                >
        >
        explicit LoggerBase(TChannel&& channel_name, TClass&& class_name, TTag&& tag_name)  :
                LoggerBase(std::forward<TChannel>(channel_name), std::forward<TClass>(class_name))
        {
            using tag_t = typename std::conditional_t<
                    std::is_convertible_v<TTag &&, std::string>,
                    std::string,
                    typename std::decay_t<TTag>>;
            impl_.add_attribute(name::Tag, impl::attrs::constant<tag_t>(std::forward<TTag>(tag_name)));
        }

        LoggerBase(LoggerBase const&) = default;
        LoggerBase(LoggerBase&&) noexcept = default;

        LoggerBase& operator=(LoggerBase const&) = default;
        LoggerBase& operator=(LoggerBase&&) noexcept = default;

        virtual ~LoggerBase();

        void trace(std::string const &message);

        void debug(std::string const &message);

        void info(std::string const &message);

        template<class TFmt, class ... Args>
        void info_f(TFmt&& fmt, Args&& ...args);

        void warning(std::string const &message);

        void error(std::string const &message);

        void fatal(std::string const &message);

    private:
        BaseType impl_;

        using text_sink = asynchronous_sink<impl::sinks::text_ostream_backend>;
        shared_ptr<text_sink> sink_;


        static void stop_logging(shared_ptr<asynchronous_sink<text_ostream_backend>> &sink);

        void init();

        void write(sdk::Severity level, std::string const &text);
    };

    template<bool mt>
    LoggerBase<mt>::~LoggerBase() {
        stop_logging(sink_);
    }

    template<bool mt>
    void LoggerBase<mt>::write(sdk::Severity level, std::string const &text) {
        BOOST_LOG_SEV(impl_, Utils::convert(level)) << text;
    }

    template<bool mt>
    void LoggerBase<mt>::trace(const std::string &message) {
        write(sdk::Severity::Trace, message);
    }

    template<bool mt>
    void LoggerBase<mt>::debug(const std::string &message) {
        write(sdk::Severity::Debug, message);
    }

    template<bool mt>
    void LoggerBase<mt>::info(std::string const &message) {
        write(sdk::Severity::Info, message);
    }

    template<bool mt>
    template<class TFmt, class ... Args>
    void LoggerBase<mt>::info_f(TFmt&& fmt, Args&& ...args) {
        boost::format f(fmt);
        write(sdk::Severity::Info, boost::str((f % ... % args)));
    }

    template<bool mt>
    void LoggerBase<mt>::warning(const std::string &message) {
        write(sdk::Severity::Warning, message);
    }

    template<bool mt>
    void LoggerBase<mt>::error(const std::string &message) {
        write(sdk::Severity::Error, message);
    }

    template<bool mt>
    void LoggerBase<mt>::fatal(const std::string &message) {
        write(sdk::Severity::Fatal, message);
    }

    template<bool mt>
    void LoggerBase<mt>::init() {
        using namespace impl;

        impl::logging::add_file_log(
                keywords::rotation_size = 10 * 1024 * 1024,
                keywords::time_based_rotation = impl::sinks::file::rotation_at_time_point(0, 0, 0)
        );

        impl::logging::formatter fmt = impl::expr::stream
                << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S ")
                << "<" << detail::severity << "> "
                << expr::if_(expr::has_attr(detail::channel))
                [
                    expr::stream << "(" << detail::channel << ") "
                ]
                << expr::if_(impl::expr::has_attr(detail::class_attr))
                [
                    expr::stream << "[" << detail::class_attr
                        << expr::if_(impl::expr::has_attr(detail::tag_attr))
                        [
                            expr::stream << "_" << detail::tag_attr
                        ]  << "] "
                ]
                << expr::smessage;

        auto sink = boost::make_shared<text_sink>();
        sink->locked_backend()->add_stream(
                shared_ptr<std::ostream>(&std::cout, boost::null_deleter{})
        );
        sink->locked_backend()->add_stream(
                boost::make_shared<std::ofstream>("sample.log")
        );
        sink->set_formatter(fmt);
        impl::logging::core::get()->add_sink(sink);


        impl::logging::add_common_attributes();

        impl::logging::core::get()->set_filter(
                impl::logging::trivial::severity >= impl::logging::trivial::info
        );

        sink_.swap(sink);

        stop_logging(sink);
    }

    template<bool mt>
    void LoggerBase<mt>::stop_logging(boost::shared_ptr<asynchronous_sink<text_ostream_backend> > &sink) {
        if (!sink)
            return;

        boost::shared_ptr<impl::logging::core> core = impl::logging::core::get();

        // Remove the sink from the core, so that no records are passed to it
        core->remove_sink(sink);

        // Break the feeding loop
        sink->stop();

        // Flush all log records that may have left buffered
        sink->flush();

        sink.reset();
    }

} // namespace log

#endif //ANALYTICS_LOGGERBASE_H
