#pragma once

#include"oso/oso_core.h"
#include <string>

namespace cdm {
    class BagsRealTimeDisplayInfo
    {
    public:
        BagsRealTimeDisplayInfo() = default;
        ~BagsRealTimeDisplayInfo() = default;

        BagsRealTimeDisplayInfo(const rw::oso::ObjectStoreAssembly& assembly);
        BagsRealTimeDisplayInfo(const BagsRealTimeDisplayInfo& obj);

        BagsRealTimeDisplayInfo& operator=(const BagsRealTimeDisplayInfo& obj);
        operator rw::oso::ObjectStoreAssembly() const;
        bool operator==(const BagsRealTimeDisplayInfo& obj) const;
        bool operator!=(const BagsRealTimeDisplayInfo& obj) const;

    public:
        int camera1Exposure{ 0 };
        int camera2Exposure{ 0 };
        int zhengmianzongliang{ 0 };
        int beimianzongliang{ 0 };
    };

    inline BagsRealTimeDisplayInfo::BagsRealTimeDisplayInfo(const rw::oso::ObjectStoreAssembly& assembly)
    {
        auto isAccountAssembly = assembly.getName();
        if (isAccountAssembly != "$class$BagsRealTimeDisplayInfo$")
        {
            throw std::runtime_error("Assembly is not $class$BagsRealTimeDisplayInfo$");
        }
        auto camera1ExposureItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$camera1Exposure$"));
        if (!camera1ExposureItem) {
            throw std::runtime_error("$variable$camera1Exposure is not found");
        }
        camera1Exposure = camera1ExposureItem->getValueAsInt();
        auto camera2ExposureItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$camera2Exposure$"));
        if (!camera2ExposureItem) {
            throw std::runtime_error("$variable$camera2Exposure is not found");
        }
        camera2Exposure = camera2ExposureItem->getValueAsInt();
        auto zhengmianzongliangItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$zhengmianzongliang$"));
        if (!zhengmianzongliangItem) {
            throw std::runtime_error("$variable$zhengmianzongliang is not found");
        }
        zhengmianzongliang = zhengmianzongliangItem->getValueAsInt();
        auto beimianzongliangItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$beimianzongliang$"));
        if (!beimianzongliangItem) {
            throw std::runtime_error("$variable$beimianzongliang is not found");
        }
        beimianzongliang = beimianzongliangItem->getValueAsInt();
    }

    inline BagsRealTimeDisplayInfo::BagsRealTimeDisplayInfo(const BagsRealTimeDisplayInfo& obj)
    {
        camera1Exposure = obj.camera1Exposure;
        camera2Exposure = obj.camera2Exposure;
        zhengmianzongliang = obj.zhengmianzongliang;
        beimianzongliang = obj.beimianzongliang;
    }

    inline BagsRealTimeDisplayInfo& BagsRealTimeDisplayInfo::operator=(const BagsRealTimeDisplayInfo& obj)
    {
        if (this != &obj) {
            camera1Exposure = obj.camera1Exposure;
            camera2Exposure = obj.camera2Exposure;
            zhengmianzongliang = obj.zhengmianzongliang;
            beimianzongliang = obj.beimianzongliang;
        }
        return *this;
    }

    inline BagsRealTimeDisplayInfo::operator rw::oso::ObjectStoreAssembly() const
    {
        rw::oso::ObjectStoreAssembly assembly;
        assembly.setName("$class$BagsRealTimeDisplayInfo$");
        auto camera1ExposureItem = std::make_shared<rw::oso::ObjectStoreItem>();
        camera1ExposureItem->setName("$variable$camera1Exposure$");
        camera1ExposureItem->setValueFromInt(camera1Exposure);
        assembly.addItem(camera1ExposureItem);
        auto camera2ExposureItem = std::make_shared<rw::oso::ObjectStoreItem>();
        camera2ExposureItem->setName("$variable$camera2Exposure$");
        camera2ExposureItem->setValueFromInt(camera2Exposure);
        assembly.addItem(camera2ExposureItem);
        auto zhengmianzongliangItem = std::make_shared<rw::oso::ObjectStoreItem>();
        zhengmianzongliangItem->setName("$variable$zhengmianzongliang$");
        zhengmianzongliangItem->setValueFromInt(zhengmianzongliang);
        assembly.addItem(zhengmianzongliangItem);
        auto beimianzongliangItem = std::make_shared<rw::oso::ObjectStoreItem>();
        beimianzongliangItem->setName("$variable$beimianzongliang$");
        beimianzongliangItem->setValueFromInt(beimianzongliang);
        assembly.addItem(beimianzongliangItem);
        return assembly;
    }

    inline bool BagsRealTimeDisplayInfo::operator==(const BagsRealTimeDisplayInfo& obj) const
    {
        return camera1Exposure == obj.camera1Exposure && camera2Exposure == obj.camera2Exposure && zhengmianzongliang == obj.zhengmianzongliang && beimianzongliang == obj.beimianzongliang;
    }

    inline bool BagsRealTimeDisplayInfo::operator!=(const BagsRealTimeDisplayInfo& obj) const
    {
        return !(*this == obj);
    }

}

