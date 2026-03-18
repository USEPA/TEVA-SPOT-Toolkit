

#include "teva-assess.h"

void DLLEXPORT Response(IndividualIngestionDataPtr *idp, IngestionDataPtr ingData, PDR dr,PNode node,PU u)
{
    int i;
	if(ingData->timingMode==IMFixed5 || ingData->timingMode==IMFixed2 || ingData->timingMode==IMDemand) {
		float *dos=ingData->nodeDoses[0];
		float *res=u->res;
		for(i=0;i<dr->maxsteps;i++){
			if(i==0 || dos[i] != dos[i-1])
				res[i]=DoseResponse(dos[i],dr);
			else
				res[i]=res[i-1];
		}
	} else {
		float *res=u->res;
		int p,pop=(int)node->pop;

		
		memset(res,0,sizeof(float)*dr->maxsteps);
		for(p=0;p<pop;p++) {
			float pres;
			for(i=0;i<dr->maxsteps;i++){
				float *dos=ingData->nodeDoses[p];
				IndividualIngestionDataPtr iidp=idp[p];
				if(i==0 || dos[i] != dos[i-1]) {
					pres=DoseResponse(dos[i],dr);
				}
				res[i]+=pres;
			}
		}
		for(i=0;i<dr->maxsteps;i++){
			res[i] = res[i]/pop;
		}
	}

}
