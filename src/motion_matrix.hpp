
using namespace Parfait;

inline MotionMatrix::MotionMatrix()
{
	clearMotions();
}

inline MotionMatrix::MotionMatrix(double dx,double dy,double dz)
{
	double translation[3];
	translation[0] = dx;
	translation[1] = dy;
	translation[2] = dz;
	setTranslation(translation);
}

inline MotionMatrix::MotionMatrix(const double translation[3])
{
	setTranslation(translation);
}


inline MotionMatrix::MotionMatrix(const double line_start[3],const double line_end[3],double rotation_angle)
{
	setRotation(line_start,line_end,rotation_angle);
}

inline void MotionMatrix::clearMotions()
{
	// sets the motion matrix to the identity matrix
	for(int i=0;i<16;i++)
		mat[i] = 0.0;
	for(int i=0;i<4;i++)
		mat[4*i+i] = 1.0;
}

inline void MotionMatrix::addTranslation(const double translation[3])
{
	MotionMatrix tran;
	tran.setTranslation(translation);
	// multiply matrices
	double A[16],result[16];
	tran.getMatrix(A);
	MatrixMatrixMultiply(A,mat,4,4,4,4,result);	
	setMotionMatrix(result);
}

inline void MotionMatrix::addRotation(const double line_start[3],const double line_end[3],double angle)
{
	MotionMatrix rot;
	rot.setRotation(line_start,line_end,angle);
	// multiply matrices
	double A[16],result[16];
	rot.getMatrix(A);
	MatrixMatrixMultiply(A,mat,4,4,4,4,result);
	setMotionMatrix(result);
}

inline void MotionMatrix::addMotion(const MotionMatrix& motion){
    double A[16],result[16];
    motion.getMatrix(A);
    MatrixMatrixMultiply(A,mat,4,4,4,4,result);
    setMotionMatrix(result);
}

inline void MotionMatrix::setTranslation(const double translation[3])
{
	clearMotions();
	mat[3] = translation[0];
	mat[7] = translation[1];
	mat[11]= translation[2];
}

inline void MotionMatrix::setRotation(const double line_start[3],const double line_end[3],double angle)
{
	double a,b,c,u,v,w,L,sin_theta,cos_theta,theta,pi;
	a = line_start[0];
	b = line_start[1];
	c = line_start[2];
	u = line_end[0] - a;
	v = line_end[1] - b;
	w = line_end[2] - c;
	L = u*u + v*v + w*w;

	pi = acos(-1.0);
	theta = angle*pi/180.0;
	sin_theta = sin(theta);
	cos_theta = cos(theta);
	
	mat[0]  = (u*u+(v*v+w*w)*cos_theta)/L;
	mat[1]  = (u*v*(1.0-cos_theta)-w*sqrt(L)*sin_theta)/L;
	mat[2]  = (u*w*(1.0-cos_theta)+v*sqrt(L)*sin_theta)/L;
	mat[3]  = ((a*(v*v+w*w)-u*(b*v+c*w))*(1.0-cos_theta)+(b*w-c*v)*sqrt(L)*sin_theta)/L;
	mat[4]  = (u*v*(1.0-cos_theta)+w*sqrt(L)*sin_theta)/L;
	mat[5]  = (v*v+(u*u+w*w)*cos_theta)/L;
	mat[6]  = (v*w*(1.0-cos_theta)-u*sqrt(L)*sin_theta)/L;
	mat[7]  = ((b*(u*u+w*w)-v*(a*u+c*w))*(1.0-cos_theta)+(c*u-a*w)*sqrt(L)*sin_theta)/L;
	mat[8]  = (u*w*(1.0-cos_theta)-v*sqrt(L)*sin_theta)/L;
	mat[9]  = (v*w*(1.0-cos_theta)+u*sqrt(L)*sin_theta)/L;
	mat[10] = (w*w+(u*u+v*v)*cos_theta)/L;
	mat[11] = ((c*(u*u+v*v)-w*(a*u+b*v))*(1.0-cos_theta)+(a*v-b*u)*sqrt(L)*sin_theta)/L;
	mat[12] = 0.0;
	mat[13] = 0.0;
	mat[14] = 0.0;
	mat[15] = 1.0;
}

inline void MotionMatrix::setMotionMatrix(const double matrix[16])
{
	for(int i=0;i<16;i++)
		mat[i] = matrix[i];
}

inline void MotionMatrix::getMatrix(double matrix[16]) const
{
	for(int i=0;i<16;i++)
		matrix[i] = mat[i];
}

inline void MotionMatrix::movePoint(double p[3]) const
{
	double vec_in[4],vec_out[4];
	vec_in[0] = p[0];
	vec_in[1] = p[1];
	vec_in[2] = p[2];
	vec_in[3] = 1.0;
	MatrixVectorMultiply(mat,vec_in,vec_out,4,4);
	p[0] = vec_out[0];
	p[1] = vec_out[1];
	p[2] = vec_out[2];
}

inline void MotionMatrix::printMatrix(FILE *f)
{
	fprintf(f,"\n%lf %lf %lf %lf\n",mat[0],mat[1],mat[2],mat[3]);
	fprintf(f,"%lf %lf %lf %lf\n",mat[4],mat[5],mat[6],mat[7]);
	fprintf(f,"%lf %lf %lf %lf\n",mat[8],mat[9],mat[10],mat[11]);
	fprintf(f,"%lf %lf %lf %lf\n",mat[12],mat[13],mat[14],mat[15]);
}
