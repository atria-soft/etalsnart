/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */

#include <etranslate/debug.hpp>

int32_t etranslate::getLogId() {
	static int32_t g_val = elog::registerInstance("etranslate");
	return g_val;
}
