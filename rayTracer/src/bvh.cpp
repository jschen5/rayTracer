#include <iostream>
#include "bvh.h"
#include "Sphere.h"


#ifndef INF 
#define INF std::numeric_limits<float>::infinity()
#endif
#ifndef nINF
#define nINF -1 * std::numeric_limits<float>::infinity()
#endif

void printVec3Beu (const glm::vec3 puttr) {
    std::cout << puttr[0] << ", " << puttr[1] << ", " << puttr[2]  << std::endl ;
}


BVH::BVH(void)
{
}

BVH::BVH(std::vector<Item*> objs, GLuint axis) {
    
    bool allMPSame = true ;
    GLuint tVecSize = objs.size() ;
    if (tVecSize == 0) {
        return ;
    } else if (tVecSize == 1) {
        left = objs[0] ;
        right = NULL ;
        bb = left->box ;
    } else if (tVecSize == 2) {
        left = objs[0] ;
        right = objs[1] ;
        bb = combine(left->box, right->box) ;
    } else {
        GLfloat aggMP = 0 ;
        GLfloat previousMP = 0 ;
        for (int i = 0; i < objs.size(); i++) {
            Item* curr = objs[i] ;
            GLfloat temp = ((curr->box.boxMin + curr->box.dVector)[axis] + curr->box.boxMin[axis]) / 2.0f ;
            if (allMPSame) {
                if (i != 0) {
                    if (previousMP != temp) {
                        allMPSame = false ;
                    }
                }
				previousMP = temp ;
            }
            aggMP += temp ;
        }
        GLfloat avgMP = aggMP / (float) objs.size() ;
        std::vector<Item*> leftVec ;
        std::vector<Item*> rightVec ;
        BoundBox tb1 ;
        BoundBox tb2 ;
        for (int i = 0; i < objs.size(); i++) {
            Item* curr = objs[i] ;
            GLfloat temp = ((curr->box.boxMin + curr->box.dVector)[axis] + curr->box.boxMin[axis]) / 2.0f ;
            if (temp <= avgMP) {
                if (allMPSame) {
                    if (i < objs.size() / 2) {
                        leftVec.push_back(curr) ;
                        if (leftVec.size() == 1) {
					        tb1 = curr->box ;
                        } else {
					        tb1 = combine(tb1, curr->box) ;
                        }
                    } else {
                        rightVec.push_back(curr) ;
                        if (rightVec.size() == 1) {
					        tb2 = curr->box ;
                        } else {
					        tb2 = combine(tb2, curr->box) ;
                        }
                    }
                } else {
                    leftVec.push_back(curr) ;
                    if (leftVec.size() == 1) {
					    tb1 = curr->box ;
                    } else {
					    tb1 = combine(tb1, curr->box) ;
                    }
                }
            } else {
                rightVec.push_back(curr) ;
                if (rightVec.size() == 1) {
					tb2 = curr->box ;
                } else {
					tb2 = combine(tb2, curr->box) ;
                }
            }
        }
        left = new BVH(leftVec, (axis + 1) % 3) ;
        right = new BVH(rightVec, (axis + 1) % 3) ;
		bb = combine(tb1, tb2);
    }
}



bool BVH::intersectionTest(RayRecord &rr, GLfloat boundT) {
    if (hitBox(rr)) {
        RayRecord lrec (rr.getT(), *rr.getMaterial(), rr.getP0(), rr.getP1()) ;
        RayRecord rrec (rr.getT(), *rr.getMaterial(), rr.getP0(), rr.getP1()) ;
        
        bool lhit, rhit ;
        
        lhit = (left != NULL) && (left->intersectionTest(lrec, boundT)) ;
        rhit = (right != NULL) && (right->intersectionTest(rrec, boundT)) ;
        if (lhit && rhit) {
            if (lrec.getT() < rrec.getT()) {
                rr = lrec ;
            } else {
                rr = rrec ;
            }
            return true ;
        } else if (lhit) {
            rr = lrec ;
            return true ;
        } else if (rhit) {
            rr = rrec ;
            return true ;
        } else {
            return false ;
        }
    } else {
        return false ;
    }

}

BoundBox BVH::combine(BoundBox b1, BoundBox b2) {
    glm::vec3 b1Max = b1.boxMin + b1.dVector ;
    glm::vec3 b2Max = b2.boxMin + b2.dVector ;
    glm::vec3 b1Min = b1.boxMin ;
    glm::vec3 b2Min = b2.boxMin ;
    
    glm::vec3 newMax = glm::vec3(glm::max(b1Max.x, b2Max.x), glm::max(b1Max.y, b2Max.y), glm::max(b1Max.z, b2Max.z)) ;
    glm::vec3 newMin = glm::vec3(glm::min(b1Min.x, b2Min.x), glm::min(b1Min.y, b2Min.y), glm::min(b1Min.z, b2Min.z)) ;
    glm::vec3 newVect = newMax - newMin ;
    
	return BoundBox (newMin, newVect) ;
}

bool BVH::hitBox(RayRecord &rr) {
    glm::vec3 p0 = rr.getP0() ;
    glm::vec3 p1 = rr.getP1() ;

    glm::vec3 bbMin = bb.boxMin ;
    glm::vec3 bbMax = bb.boxMin + bb.dVector ;
    glm::vec3 a = glm::vec3(1, 1, 1) / p1 ;
    GLfloat tmin, tmax, tymin, tymax, tzmin, tzmax ;
    if (a[0] >= 0) {
        tmin = (bbMin[0] - p0[0]) * a[0] ;
        tmax = (bbMax[0] - p0[0]) * a[0] ;
    } else {
		tmin = (bbMax[0] - p0[0]) * a[0];
		tmax = (bbMin[0] - p0[0]) * a[0];
	}
	if (a[1] >= 0) {
		tymin = (bbMin[1] - p0[1]) * a[1];
		tymax = (bbMax[1] - p0[1]) * a[1];
	} else {
		tymin = (bbMax[1] - p0[1]) * a[1];
		tymax = (bbMin[1] - p0[1]) * a[1];
	}
	if (tmin > tymax || tymin > tmax) {
		return false;
	}
	if (tymin > tmin) {
		tmin = tymin;
	}
	if (tymax < tmax) {
		tmax = tymax;
	}
	if (a[2] >= 0) {
		tzmin = (bbMin[2] - p0[2]) * a[2];
		tzmax = (bbMax[2] - p0[2]) * a[2];
	} else {
		tzmin = (bbMax[2] - p0[2]) * a[2];
		tzmax = (bbMin[2] - p0[2]) * a[2];
	}
	if (tmin> tzmax || tzmin > tmax) {
		return false;
	}
	if (tzmin > tmin) {
		tmin = tzmin;
	}
	if (tzmax < tmax) {
		tmax = tzmax;
	}
	return (tmin < rr.getT() && tmax > 0);
}

BoundBox BVH::makeBox () {
    return BoundBox();
}

BVH::~BVH(void)
{
}
