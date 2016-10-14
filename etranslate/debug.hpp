/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <elog/log.hpp>

namespace etranslate {
	int32_t getLogId();
};
#define ETRANSLATE_BASE(info,data) ELOG_BASE(etranslate::getLogId(),info,data)

#define ETRANSLATE_PRINT(data)         ETRANSLATE_BASE(-1, data)
#define ETRANSLATE_CRITICAL(data)      ETRANSLATE_BASE(1, data)
#define ETRANSLATE_ERROR(data)         ETRANSLATE_BASE(2, data)
#define ETRANSLATE_WARNING(data)       ETRANSLATE_BASE(3, data)
#ifdef DEBUG
	#define ETRANSLATE_INFO(data)          ETRANSLATE_BASE(4, data)
	#define ETRANSLATE_DEBUG(data)         ETRANSLATE_BASE(5, data)
	#define ETRANSLATE_VERBOSE(data)       ETRANSLATE_BASE(6, data)
	#define ETRANSLATE_TODO(data)          ETRANSLATE_BASE(4, "TODO : " << data)
#else
	#define ETRANSLATE_INFO(data)          do { } while(false)
	#define ETRANSLATE_DEBUG(data)         do { } while(false)
	#define ETRANSLATE_VERBOSE(data)       do { } while(false)
	#define ETRANSLATE_TODO(data)          do { } while(false)
#endif

#define ETRANSLATE_ASSERT(cond,data) \
	do { \
		if (!(cond)) { \
			ETRANSLATE_CRITICAL(data); \
			assert(!#cond); \
		} \
	} while (0)

