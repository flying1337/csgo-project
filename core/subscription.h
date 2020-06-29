//
// Created by ruppet on 2/5/2020.
//

#ifndef uc-sflying-loader-subscription-h
#define uc-sflying-loader-subscription-h

class c_subscription {
public:
    int product_id = 0;
    int status = 0;

    std::string sub_name;
    std::string last_update;
    std::string expire_date;
    std::string process_name;
    std::string steamid;
};

#endif //uc-sflying-loader-subscription-h
