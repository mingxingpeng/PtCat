//
// Created by 27852 on 2026/5/25.
//

#ifndef PTCAT_PTEXTSTREAM_H
#define PTCAT_PTEXTSTREAM_H

#include "pfstream.h"

namespace ptcat {
    namespace fstream {
        class DLL_API PTextStream : public PFStream {
        public:
            PTextStream() = default;

            ~PTextStream() = default;

            bool WriteLineSync(const std::string& text);

            bool ReadLineSync(std::string& text);
        };

    }
}

#endif //PTCAT_PTEXTSTREAM_H
