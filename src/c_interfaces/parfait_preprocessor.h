//
// Created by cdruyorj on 8/20/15.
//

#ifndef PARFAIT_PARFAIT_PREPROCESSOR_H
#define PARFAIT_PARFAIT_PREPROCESSOR_H

#ifdef __cplusplus
extern "C" {
#endif

void parfait_create_grid_system(const char* filename);
int parfait_preprocessor_number_of_local_nodes();
void parfait_cleanup();

#ifdef __cplusplus
};
#endif

#endif //PARFAIT_PARFAIT_PREPROCESSOR_H
