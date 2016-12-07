% ---------------------------------------------------------------------
% EG author guidelines plus sample file for EG publication using LaTeX2e input
% D.Fellner, v1.17, Sep 23, 2010


%\title[EG Astronomical Scale Differences and Visualization]%
%      {Astronomical Scale Differences in Visualization}
% How about this? ---abock
\title[]{Dynamic Scene Graphs: Avoiding Floating Point Inaccuracies in Large Scale Astrovisualization}

% for anonymous conference submission please enter your SUBMISSION ID
% instead of the author's name (and leave the affiliation blank) !!
\author[Axelsson \emph{et al.}]{Emil Axelsson$^{1}$, Jonathas Costa$^{2}$, Alexander Bock$^{1}$, Cl\'audio Silva$^{2}$, and Anders Ynnerman$^{1}$
        \\
% For Computer Graphics Forum: Please use the abbreviation of your first name.
         $^1$ Linköping University\\
         $^2$ New York University
       }

% ------------------------------------------------------------------------

% if the Editors-in-Chief have given you the data, you may uncomment
% the following five lines and insert it here
%
% \volume{27}   % the volume in which the issue will be published;
% \issue{1}     % the issue number of the publication
% \pStartPage{1}      % set starting page


%-------------------------------------------------------------------------
\begin{document}

\maketitle

\begin{abstract}
In this work, we address the challenges of limited numerical range and precision of floating point numbers when simultaneously visualizing astronomical objects with huge scale differences, both in size and distance. Due to the limited precision of floating point numbers, it is impossible to represent comparatively small objects far away from the coordinate system origin. We propose a system that utilizes a dynamically assigned coordinate system origin in a scene graph to provide the highest possible numerical precision for all salient objects. This makes it possible to interactively render, for example, surface structures on Mars and the Milky Way simultaneously. We track and quantify the propagation of numerical precision errors through the computer graphics pipeline using interval arithmetic for cases of large differences in scale. Furthermore, we identify sources of precision degradation, leading to incorrect object positions in screen-space and z-fighting, mathematically. Our proposed method operates without near and far planes while maintaining high depth precision through the use of floating point depth buffers. By providing interoperability with order-independent transparency algorithms, direct volume rendering, and stereoscopy, our approach is well suited for scientific visualization. We provide the mathematical background, a thorough description of the method, and a reference implementation.
\end{abstract}

%-------------------------------------------------------------------------
\section{Introduction}
Maybe one of the biggest achievements of mankind is the ability to observe and collect data about the surrounding cosmos. This includes observations ranging from the diminutive to the unimaginably large. Everything we know and everything we can possibly know is constrained in size and distance by two limits. The upper limit is given by the size of the observable universe with a comoving diameter of about $10^{27}$ meters. The lower limit is the Planck length, at which the structure of space-time is dominated by quantum effects and the concept of locality loses physical meaning, with an approximate size of $10^{-35}$ meters. These two limits pose absolute limits to the resolution that a software system dealing data of any size has to support. While impressive, this tremendous range of scales over roughly 60 orders of magnitude generates many challenges to visualization system operating across such scales. Besides the issue of crossing different abstraction levels for the visualization, one challenge not immediately obvious is related to the precision of floating point numbers. Following a na\"ive approach using a conventional computer graphics pipeline object locations cannot be described with the necessary precision, for example when viewing models of space craft at the edge of our solarsystem, or watching micrometer-scaled scans on the surface of Mars. 

%Through various means of scientific discovery, humanity now has collected detailed information and measurements of structures in the surrounding cosmos that range from the diminutive to the unimaginably large. Everything we know and everything we can possibly know is constrained in size and distance by two boundaries. The upper boundary is given by the size of the observable universe with a comoving diameter of about $10^{27}$ meters. The lower boundary is the Planck length, at which the structure of space-time is dominated by quantum effects and the concept of locality loses physical meaning, with an approximate size of $10^{-35}$ meters. These two boundaries pose absolute limits to the resolution that a software system dealing data of any size has to support.

%Thanks to humanity's collective discoveries of the surrounding world, we are now aware of structures in the universe that are both tiny and huge. The observable universe has a diameter of approximately $10^{27}$ meters, while the Planck length, which is the length where quantum mechanical effects make it impossible to distinguish between two physical locations \joncomment{Maybe: ``...which is the length were the Heisenberg's uncertainty principle is present/observable, is...''}, is approximately $10^{-35}$ meters.

When virtually exploring datasets with these vast scale differences and distances, the location of objects need to be expressed in huge value ranges with great precision. Current computer hardware is limited to hardware accelerated computations of single and double precision floating point numbers, which are insufficient to represent microscopic objects and cosmological distances simultaneously. This lack of precision causes visual rendering artifacts, such as displaced vertices or z-fighting, and generate problems when displaying volumetric content. Furthermore, overflow and underflow of the available value ranges results in undesirable clipping of near and far geometry.

Previous work by the visualization community has provided solutions to some of these issues, such as the Power Scaled Coordinates by Hanson~\etal~\cite{hanson2000very} and the ScaleGraph by Klashed~\etal~\cite{KHECY10}. However, only partial solutions to the precision issues have been provided thus far and the root cause of these problems remains unsolved.

In this work, we propose a general framework, a dynamic scene graph, to deal with multiple scales or varying sizes within one application. The concept is applicable to large variety of settings. This framework is constructed by analyzing the sources of precision errors and range overflow problems in a conventional computer graphics pipeline. Using interval arithmetic to track floating point rounding errors, we are able to identify which set of operations that may introduce precision problems. We combine this information with the observation that high detail is only required for close objects, we present and motivate our method for visualizing datasets with astronomical scale and distance differences. This techniques relies on a dynamic update and traversal of a scene graph whereby cameras automatically attach and detach to the closest object of interest. This object of interest is then used as a new coordinate origin and, thus, provided the highest possible numerical precision. 

The dynamic scene graph has been implemented in the \emph{OpenSpace} project. OpenSpace is an open source software project with the goal of interactively visualizing and contextualizing our knowledge of the known universe and our ongoing efforts of exploration. The software framework is designed for immersive environments to the public dissemination of science and to contextualize information if all possible scales, such as sub-atomic particle simulations on Earth, micrometer-scale discoveries made by rovers the Martian surface, or volumetric simulations of entire galaxies. In order to fully utilize the potential of these immersive environments, for example planetariums or virtual reality headsets, the software needs to support high-fidelity stereoscopic rendering and the ability to render on multipipeline architectures, such as power walls. However, our method is applicable to any visualization tool that uses scene graphs to represent objects, and is compatible with techniques for volumetric rendering as well as order-independent transparency techniques.

In summary, our contributions include:

\begin{itemize}
\item A \emph{Dynamic Scene Graph} approach for representing objects and cameras to avoid precision-related rendering artifacts.
\item A general method for rendering objects with a wide depth range and precision without the explicit need for near and far planes.
\item A scheme for seamless adaption of the eye-separation used for stereoscopic rendering.
\end{itemize}

\section{Related work}
Computer graphics and interactive visualization have
proven to be immensely valuable tools for communicating scientific findings
and contextualizing information. Notable examples of applications where huge scale differences are visualized 
together include movie productions, such as \emph{Powers of Ten} by Eames~\cite{powersOfTen} from 1977,
All We Are [citation needed], as well as interactive software for digital planetariums,
such as Uniview [citations], DigiStar [citations], DigitalSky [citation], or free software such as Celestia [citations] or SpaceEngine.

The concept of \emph{Power Scaled Coordinates}, introduced by Hanson~\etal ,
addresses some of the challenges with multi-scale visualization~\cite{hanson2000very}. The method uses four-dimensional floating point vectors where the first three components determine the direction of the vector, and the fourth encodes a scaling coefficient as a power of ten. The proposed system can operate with floating point numbers close to unit scale, which solves the issue of exponent overflow that occurs when performing operations on large and close-to-zero vectors.

\alexcomment{Duplicate of the previous section?} The Power Scaled Coordinates method is based on floating point representation of numbers, which yields high precision close to zero, and decreasing precision with larger values. Hence, the method achieves high precision close to the origin of the scene, but decreasing precision for coordinates further away. This is a viable solution for scenes where the high precision data is centered in one region, such as solely close to Earth. However, the challenge of representing multiple regions in the virtual universe with high precision is not addressed in this work.

Fu~\etal \cite{fu2007transparently} elaborate on the concept of Power Scales Coordinates and introduce a depth buffer remapping scheme to cover a wider range of distances than what is possible with a fixed point depth buffer and a conventional near and far plane. The depth buffer range is divided into three regions, where small and huge depth values are remapped using logarithmic scales, while values in the mid-range are mapped linearly. While this method may be beneficial for some specific type of scene content, the method does not provide a generic way to select appropriate threshold values, and the choice of three separate regions with different mapping does not necessarily provide better results than one single logarithmic mapping.

The \emph{ScaleGraph} method, introduced by Klashed~\etal \cite{KHECY10} as a part of the digital planetarium software Uniview, employs a scene graph structure with sub-scenes that the camera can be attached to. The content of the current sub-scene is rendered in its local coordinate system, which maintains the precision of vertex coordinates, and allows multiple regions in the universe to be represented with high relative precision. When rendering content outside the current sub-scene, objects are translated onto the bounding sphere of the scene and scaled to compensate for perspective effects. Vectors between sub-scenes are computed by traversing the scene graph through the closest common ancestor in the tree, to avoid introducing precision errors caused by handling unnecessarily large numbers. By moving objects onto the bounding sphere of the current scene, object depths are mapped to a manageable range. However, the method does not provide a general solution for depth sorting objects that are outside the current sub-scene. Furthermore, the work does not address stereoscopic rendering of objects across sub-scenes. The traversal of sub-scenes in this work formed the bases for our algorithm of the dynamic traversal of a scene graph.

\section{Theoretical foundation}\label{sec:theoretical}
The co-evolution of graphics hardware, software, and content, largely pushed forward by
the gaming industry, has led to modern graphics processing units that are optimized for
operating efficiently on floating point numbers, vectors, and matrices.
In a standard computer graphics pipeline, $4x4$ matrices are used to represent coordinate
transformations operating on 4-dimensional homogeneous coordinate vectors with floating point components. In order to understand the precision and range limitations of the graphics pipeline used in visualization, we need to base our reasoning on properties on floating point numbers and the operations that we perform on them.

\begin{figure}
\includegraphics[width=8.4cm,height=8.4cm,keepaspectratio]{degradation-circles.pdf}
\caption{5 mantissa bits instead of 23. TODO: write more here}
\label{fig:degradation-circles}
\end{figure}

\subsection{Floating point numbers}
The widely used IEEE754 standard for representing 32-bit floating point numbers \cite{zuras2008ieee} enables the possibility to express numbers in a much larger range than what is possible using 32-bit integers. While a 32-bit signed integer type can only represent numbers in the range $[-2^{31},\, 2^{31} - 1]$, an IEEE754 single precision floating point number approximately supports the range $[-2^{127},\, 2^{127}]$.

In floating point data types, values are represented as a sign, a mantissa, and an exponent. This representation has the attributes of resulting in high precision close to zero, decreasing precision for larger numbers, and a dramatic increase in value range. The IEEE754 single precision floating point format uses 1 sign bit, 8 exponent bits, and 23 mantissa bits. Equation \ref{eq:floatingpoint} yields the value $v$ represented by the sign bit $s$, the mantissa bits $m_i$, and the integer $e \in [-127, 128]$ which is encoded in the exponent bits. 

\begin{equation} \label{eq:floatingpoint}
v = \begin{cases}
(-1)^s \sum_{i = 1}^{23}m_i\,2^{-i} 2^{-126}, & \text{for } e = -127 \\
(-1)^s (1 + \sum_{i = 1}^{23}m_i\,2^{-i}) 2^e, & \text{for } -126 \leq e \leq 127 \\
\text{Special numbers such as NaN or }\infty, & \text{for } e = 128
\end{cases}
\end{equation}

\noindent Only values that can be composed by this set of bits are valid 32-bit floating point numbers. If exponent of a number is not in the range $[-126, 127]$ it will be represented as $\infty$ or $-\infty$. One critical characteristic of IEEE754 floating point numbers $\mathbb{R}_f$ \alexcomment{Is there a better symbol for this?} is their inability to represent all possible numbers $\mathbb{R}$. All numbers $\mathbb{R}_f \setminus \mathbb{R}$ that are not representable are rounded to their closeset \alexcomment{is this correct?} representable number in $\mathbb{R}_f$.

An alternative data type for representing numbers is the IEEE754 64-bit floating-point format (double precision). The standard uses 1 sign bit, 11 exponent bits and 52 bits for the mantissa. Using 128-bits (quadruple precision), it is possible to further increase the precision and exponent range. Increasing the bit depth will naturally require more computational resources, and while the OpenGL Shading Language (GLSL) natively supports the doubles, there is no quad-precision data type available as of GLSL 4.4 \cite{kessenich2014opengl}. 

\subsubsection{Precision limitations}
Due to the limited number of mantissa bits, values need to be rounded in order to be represented as floating point numbers. An upper bound of the absolute spacing between a floating point number $v$ and an adjacent one can be expressed as $\epsilon|v|$\cite{higham2002accuracy}, where the \emph{machine epsilon} $\epsilon$ is a constant given by $2^{-(n + 1)}$, and $n$ is the number of mantissa bits. Hence, the maximum error introduced by rounding of $v$ can be written as $u|v|$, where the rounding error coefficient $u = \frac{\epsilon}{2}$.
For IEEE754 float, $\epsilon = 2^{-24}\approx 5.96 \cdot 10^{-8}$, meaning that values are precise to about 7 significant figures in decimal notation.

Double precision floating point numbers have a machine epsilon of $\epsilon = 2^{-53}\approx 1.11 \cdot 10^{-16}$. In decimal notation, this corresponds to a precision of 15 significant figures. For quadruple precision the corresponding number is 34 significant figures.

\subsubsection{Exponent underflow and overflow}
The exponent range $[-126, 127]$ in 32-bit IEEE754 floats gives a possible scale range of approximately 76 orders of magnitude (OM). Computations that yield exponents outside that range will cause exponent underflow or overflow, and result in loss of data. A straight-forward implementation of computing the length of a vector using the Pythagorean theorem would need to store squared length in a floating point value as an intermediate step, halving the range of valid input exponents and limiting the input scale differences to 38 OM.

The scale difference between the Planck length and the diameter of the observable universe is approximately $62$ OM. While 32-bit floats are able to represent this scale difference, a conventional approach to computing lengths and normalizing vectors would break down.


\subsection{Interval arithmetic}
To reason about the precision of various floating point operations, it is possible to use interval arithmetic as a mathematical tool to track possible rounding errors. Due to the upper bound of the rounding error, a value $v$ may be rounded to a number $v' \in [v - u|v|, v + u|v|]$. Using interval arithmetic, the interval can be rewritten as $v(1 + u[-1,1])$.


Throughout the following sections, $\oplus$, $\ominus$, $\odot$ and $\oslash$ will be used to denote floating point addition, subtraction, multiplication and division respectively. The IEEE754 standard requires that the result of each of these operations is identical to the result of an infinitely precise computation followed by a rounding to the closest representable float. The operators are commutaive, and yield the possible output intervals given two real number intervals $[x]$ and $[y]$ as described in equation \ref{eq:floatoperations}.

\begin{equation} \label{eq:floatoperations}
\begin{split}
[x] \oplus [y] & \subseteq ([x] + [y])(1 + u[-1, 1]) \\
[x] \ominus [y] & \subseteq ([x] - [y])(1 + u[-1, 1]) \\
[x] \odot [y] & \subseteq [x][y](1 + u[-1, 1]) \\
[x] \oslash [y] & \subseteq \frac{[x]}{[y]}(1 + u[-1, 1]) \\
\end{split}
\end{equation}

For operations involving the degenerate intervals $[0, 0]$ and $[1, 1]$, we observe some special cases:
\begin{equation} \label{eq:floatspecialcases}
\begin{split}
[x] \odot [0, 0] & = [0, 0] \\
[x] \odot [1, 1] & = [x] \\
[x] \oplus [0, 0] & = [x]
\end{split}
\end{equation}


\subsection{Propagation of error intervals}
When a result from one floating point operation is used in further computations, rounding errors will propagate through the whole series of operations. This may lead to scenarios where expressions on the form $(a + b) - b$ may evaluate to $0$, for a sufficiently small $a$ and large $b$. This phenomena is known as 
\emph{catastrophic cancellation}. In any rendering pipeline where e.g. matrix multiplications or vector additions result in this type of operations, there is a risk for floating point precision problems.

Let $\mathds{E}$ be the set of intervals that can be written in form of equation $cu[-1, 1]$, where c is a non-negative constant that is not a function of any floating point value. For any intervals $[e_1], [e_2] \in \mathds{E}$, there are intervals $[e_3], [e_4] \in \mathds{E}$, such that
$[e_1]+[e_2] \subseteq [e_3]$ and $[e_1e_2] \subseteq [e_4]$.

We let $[e_i] = c_iu[-1, 1] \in \mathds{E}$ and observe some properties of $\oplus$, and $\odot$ as they act on the two intervals $a(1 + [e_1])$ and $b(1 + [e_2])$.

\begin{equation} \label{eq:errortermadd} 
\begin{split}
a(1 + [e_1]) \oplus b( 1 + [e_2]) \\
\subseteq \big(a(1 + [e_1]) + b( 1 + [e_2])\big)(1 + u[-1, 1]) \\
\subseteq a(1 + [e_3]) + b(1 + [e_4])
\end{split}
\end{equation}

\begin{equation} \label{eq:errortermmult} 
\begin{split}
a(1 + [e_1]) \odot b(1 + [e_2]) \\
\subseteq ab(1 + [e_1])(1 + [e_2])(1 + u[-1, 1]) \\
\subseteq ab(1 + [e_5])
\end{split}
\end{equation}

Since $[e]$-terms are intervals on the form $cu[-1, 1]$, we can conclude that equation \ref{eq:errortermadd} yields a maximum rounding error of $|ac_3u| + |bc_4u|$. Equation \ref{eq:errortermmult} may output a maximum rounding error of $|abc_5u|$. While several floating point additions will accumulate errors from all contributing terms, creating a risk for catastrophic cancellation, floating point multiplication will scale the error proportionally to the factors. Multiplying an interval with a number with absolute value smaller than $1$ will hence shrink the absolute error interval proportionally. The relative precision $c_5u$ is independent of the size of the factors $a$ and $b$. \emilcomment{Is this a better claim?}.


By applying equation \ref{eq:errortermadd} iteratively, we can also conclude that for floating point sums $\osum_i x_i = x_1 \oplus x_2 \oplus ... \oplus x_n$, equation \ref{eq:errortermsum} holds regardless of in which order terms are added, with $[e_i], [e'_i] \in \mathds{E}$.

\begin{equation} \label{eq:errortermsum} 
\osum_i x_i(1 + [e_i]) \subseteq \sum_i x_i(1 + [e'_i])
\end{equation}

The straight-forward implementation of floating point matrix multiplication $A \otimes B$ is to compute matrix components as a sum of products using $\oplus$ and $\odot$. By applying equations \ref{eq:errortermmult} and \ref{eq:errortermsum}, we get equation \ref{eq:matrixmult} where $m$ is the number of columns of $A$ and the number of rows of $B$.

\begin{equation} \label{eq:matrixmult} 
(A \otimes B)_{ij} \subseteq \osum_{k = 1}^mA_{ik}\odot B_{kj} \subseteq \sum_{k = 1}^mA_{ik} B_{kj}(1 + [e_{ijk}])
\end{equation}

\noindent We observe that the maximum floating point error introduced to any individual component $(AB)_{ij}$ of a matrix product is given by $\sum_{k = 1}^m|A_{ik} B_{kj} [e_{ijk}]|$.


\subsection{Coordinate transformations}
In computer graphics software, scene content is commonly organized in a \emph{scene graph}, which allows coordinate transformations to be represented hierarchically. Each node in the graph may contain a transformation, represented as a 4x4 matrix, that affects all its children. Objects consist of vertices $\mathbf{x}$ represented as homogeneous coordinates $(x, y, z, w)^t$, where $x$, $y$ and $z$ are the coordinates in a local coordinate system, and $w = 1$. In order to transform a vertex from its local \emph{model coordinate system} to a global \emph{world coordinate system}, the transformation matrices of the ancestor nodes are concatenated to a final \emph{model matrix} using matrix multiplication. This final matrix is applied to the vertex $\mathbf{x}$ to acquire the world coordinates $\mathbf{w}$.

A \emph{view matrix} is also commonly generated based on a camera position and rotation expressed in the world coordinate system, and then used to transform world coordinates to \emph{view coordinates}, with the camera in the origin, and the z-axis parallel to the view direction. This view matrix is multiplied with the world coordinate $\mathbf{w}$ and outputs the view coordinates $\mathbf{v}$.

View coordinates are transformed into \emph{clip coordinates} $\mathbf{c}$ using a \emph{projection matrix}, in this case a perspective projection matrix.
After that, content that does not satisfy the equation $-w \le x, y, z \le w$ will be subject to frustum culling, and will be discarded from rendering. The vector is divided by $w$ to compute the \emph{normalized device coordinates (NDC)}. This last step, commonly called \emph{perspective division}, causes objects farther away from the camera to appear smaller on the screen. The $x$ and $y$ in NDC determines the object's position on the screen, with $(-1, -1)$ representing the bottom left corner and $(1, 1)$ upper right one.

Depth sorting is commonly performed using a depth buffer, which stores a depth value for each pixel on the screen. Fragments are only rendered to if they are closer than the closest already rendered fragment at the same screen location. The depth value of a fragment is commonly derived from the $z$-component of the normalized device coordinates, whereas $z = 1$ represents the \emph{near plane} and and $z = -1$ represents the \emph{far plane}. \joncomment{Are you sure about these values for z? I don't remember these values by heart, but they don't look familiar to me.} If the depth buffer is not represented with 32-bit floating point numbers, the depth first needs to be converted to the appropriate type. 

\subsection{Coordinate precision}
Rounding errors in the graphics pipeline may give rise to two types of visual artifacts:
\begin{itemize}
\item If errors in the $x$ and $y$ components of the normalized device coordinates are in the same order as the size of the pixels on the screen, there will be a \emph{visible displacement of vertices}. 
\item If the rounding error of $z$ when stored in the depth buffer is larger than the $z$-difference of two adjacent fragments, there is a risk that the incorrect fragment is rendered on top, which is often referred to as \emph{z-fighting}.
\end{itemize}

By using interval arithmetic to trace the errors in $x$ and $y$ it is possible to predict vertex displacement issues. Similarly, the accumulated error interval of the z-component can be used to determine when there is a risk for z-fighting. In the following sections, we study the precision error implications of the coordinate operations in the conventional graphics pipeline to determine the set of circumstances in which precision problems may be introduced.

Using column vectors, the NDC's $\mathbf{n}$ are derived from the model coordinates $\mathbf{x}$ in equation \ref{eq:mvp}.

\begin{align} \label{eq:mvp}
\mathbf{c} = P\otimes V \otimes M \otimes \mathbf{x} \\
\mathbf{n} = \mathbf{c} \oslash {c_w}
\end{align}
 
Here, $P$ is the projection matrix, V is the view matrix, $M$ is the model matrix, and $\mathbf{x}$, $\mathbf{c}$ and $\mathbf{n}$ are homogeneous coordinate vectors on the form $(x, y, z, w)^t$. The precision in normalized device coordinates depends on the collective effects of $M$, $V$, $P$ and the perspective division as they act on $\mathbf{x}$. 

$M$ and $V$ are commonly composed from three types of transformations that are encoded in the scene graph: scaling, translation and rotation. We study the effect of applying scaling, translation and rotation on a generalized model matrix or view matrix A, composed of a translation vector $\mathbf{a}$ and a combined rotation/scaling 3x3-matrix $A'$, as given by equation \ref{eq:matrixa}. To observe how the matrices act on a coordinate vector, we can study the fourh column in A.
\begin{equation} \label{eq:matrixa}
A = \mleft(
\begin{array}{c|c}
  A' & \mathbf{a}\\
  \hline
  \mathbf{0} & 1 
\end{array}
\mright)
\end{equation}

After evaluating the possible effects of the scaling, translation and rotation, we continue with a similar analysis for the projection matrix, the perspective division and conversion of to depth buffer representation.

\subsubsection{Scaling}

Equation \ref{eq:scaling} describes a floating point matrix multiplication of a scaling matrix $S$ and a matrix $A$. $S'$ is a diagonal matrix with one scaling component per dimension $x$, $y$ and $z$. 
\begin{equation} \label{eq:scaling}
\hat{S} = S \otimes A = 
\mleft(
\begin{array}{c|c}
  S' & \mathbf{0}\\
  \hline
  \mathbf{0} & 1 
\end{array}\mright)
\otimes
\mleft(
\begin{array}{c|c}
  A' & \mathbf{a}\\
  \hline
  \mathbf{0} & 1 
\end{array}
\mright) =  
\mleft(
\begin{array}{c|c}
  \hat{S}' & \hat{\mathbf{s}}\\ 
  \hline
  \mathbf{0} & 1 
\end{array}\mright)
\end{equation}

\noindent Since the diagonal matrix components $S'_{ij}$ are $0$ for all $i \neq j$, the output matrix components $\hat{S}'_{ij}$ and $\hat{\mathbf{s}}_i $ can be written as in equation \ref{eq:scalingconclusion}.

\begin{align} \label{eq:scalingconclusion}
\hat{S}'_{ij} = (S' \otimes A')_{ij} &\subseteq S'_{ii}\,A'_{ij}(1 + [e_{ij}]) \\
\hat{\mathbf{s}}_i = (S' \otimes \mathbf{a})_{i} &\subseteq S'_{ii}\,\mathbf{a}_i(1 + [e_{j}])
\end{align}

The rounding error introduced by a scaling matrix $S$ is proportional to the scaling factors in $S'$. Any error interval encoded in $A'$ and $\mathbf{a}$ will be scaled proportionally. We conclude that applying a scaling matrix to a matrix $A$ \emph{preserves the relative precision} of all components of $A$.

\subsubsection{Translation}
In equation \ref{eq:translation}, a translation matrix $T$ is applied to $A$ using floating point matrix multiplication. $I$ denotes a 3x3 identity matrix, and $\mathbf{t}$ represents the translation vector. 

\begin{equation} \label{eq:translation}
\hat{T} = T \otimes A = 
\mleft(
\begin{array}{c|c}
  I & \mathbf{t}\\
  \hline
  \mathbf{0} & 1 
\end{array}\mright)
\otimes
\mleft(
\begin{array}{c|c}
  A' & \mathbf{a}\\
  \hline
  \mathbf{0} & 1 
\end{array}
\mright) =  
\mleft(
\begin{array}{c|c}
  \hat{T}' & \hat{\mathbf{t}}\\
  \hline
  \mathbf{0} & 1 
\end{array}\mright)
\end{equation}

where 

\begin{align} \label{eq:translationconclusion}
\hat{T}_{ij} = (I \otimes A')_{ij} = A'_{ij}\\
\hat{t}_i = t_i \oplus a_i \subseteq (t_i + a_i)(1 + [e_i])
\end{align}
We note that the precision in $\hat{T}_{ij}$ is preserved from $A'$ when any translation matrix $T$ is applied. However, any error intervals encoded in $t_i$ and $a_i$ will propagate to $\hat{t}_i$, and will \emph{not} be scaled down even if $|\hat{t}_i|$ is orders of magnitude smaller than $|t_i|$ and $|a_i|$. This scenario \emph{may lead to catastrophic cancellation} in the component $\hat{t}_i$.


\subsubsection{Rotation}
The same analysis is performed for floating point rotation in equation \ref{eq:rotation}.

\begin{equation} \label{eq:rotation}
\hat{R} = R \otimes A = 
\mleft(
\begin{array}{c|c}
  R' & \mathbf{0}\\
  \hline
  \mathbf{0} & 1 
\end{array}\mright)
\otimes
\mleft(
\begin{array}{c|c}
  A' & \mathbf{a}\\
  \hline
  \mathbf{0} & 1 
\end{array}
\mright) =  
\mleft(
\begin{array}{c|c}
  \hat{R}' & \hat{\mathbf{r}}\\
  \hline
  \mathbf{0} & 1 
\end{array}\mright)
\end{equation}

\begin{align} \label{eq:rotationconclusion}
\hat{R}'_{ij} = (R' \otimes A')_{ij} &\subseteq \sum_{k = 1}^3 R'_{ik}\,A'_{kj}(1 + [e_{ijk}]) \\
\hat{\mathbf{r}}_i = (R' \otimes \mathbf{a})_{i} &\subseteq \sum_{k = 1}^3 R'_{ik}\,\mathbf{a}_k(1 + [e_{ijk}])
\end{align}

When applying a rotation matrix to a general matrix $A$, the potential rounding error of any output component $A_{ij}$ will be equal to $\sum_{k = 1}^3 |R'_{ik}\,A'_{kj} [e_{ijk}]|$. For a general rotation matrix $R$ written on the same form as in \ref{eq:rotation}, all the components of $R'$ are in the interval $[-1, 1]$, which means that the maximum possible precision loss is governed by the values of A. When applying $R$ on a matrix or vector with both huge and close-to-zero components, the precision in the small components may be lost. However, for the most significant matrix components, the relative precision will be close to preserved. This means that when $R$ is used to transform the coordinate vector $\mathbf{a} = (x, y, z)^t = \mathbf{\hat{a}}|\mathbf{a}|$, neither the precision of the direction $\mathbf{\hat{a}}$ nor the magnitude $|\mathbf{a}|$ will be affected significantly. The same is true for the column vectors in $A'$, which means that the significant properties of $A'$ are preserved in $\hat{R}'$

\subsubsection{Projection}
A typical matrix for symmetric perspective projection $P$ can be written as in equation \ref{eq:perspectiveprojection}, where
$\theta_h$ and $\theta_v$ are the horizontal and vertical field of view, and $n$ and $f$ are the distances from the camera to the near and far planes.

\begin{equation} \label{eq:perspectiveprojection}
P = \mleft(
\begin{array}{cccc}
  \arctan(\frac{\theta_{h}}{2}) & 0 & 0 & 0\\
  0 & \arctan(\frac{\theta_{v}}{2}) & 0 & 0\\
  0 & 0 & -\frac{f+n}{f-n} & \frac{2fn}{f-n}\\
  0 & 0 & -1 & 0
\end{array}\mright)
\end{equation}

\joncomment{Why did you prefer to use the version with $arctan$ instead of the version with $\frac{2n}{r-l}$ and $\frac{2n}{t-b}$ elements? Any reason in particular?}

When $P$ is multiplied with a homogeneous view coordinate vector $\mathbf{v} = (x, y, z, 1)^t$, the sums of products to compute the output x, y and w components will only consist of one non-zero term, which means that the relative precision of these components is preserved. The z-component $c_z = (P\mathbf{v})_z$ is given by equation \ref{eq:nearfar}.

\begin{equation} \label{eq:nearfar}
\begin{split}
\lambda_1 = -\frac{f + n}{f - n} \\
\lambda_2 = \frac{2fn}{f - n} \\
c_z = \lambda_1 v_z + \lambda_2
\end{split}
\end{equation}

To observe the effect of a huge scale difference between $f$ and $n$ on the precision of $c_z$,
we study equation \ref{eq:nearfar} when $f \rightarrow \infty$ and $n \rightarrow 0$, and see that 
$\lambda_1 \rightarrow -1$ and $\lambda_2 \rightarrow 0$ respectively, which means that $c_z \rightarrow -v_z = c_w$.
The perspective division step would yield $n_z = 1$ for all vertices, giving them the exact same depth buffer value.

\subsubsection{Perspective division}
The perspective division, carried out to bring coordinates from clip space to normalized device coordinates will divide all vector components with the $w$-component. When using a perspective projection matrix as described in equation \ref{eq:perspectiveprojection}, all components will effectively be divided with the view coordinates' $-z$. Since floating point division preserves the relative precision, the absolute error interval will scale proportionally with the value itself.
The perspective division, carried out to bring coordinates from clip space to normalized device coordinates will divide all vector components with the $w$-component. When using a perspective projection matrix as described in equation \ref{eq:perspectiveprojection}, all components will effectively be divided with the view coordinates' $-z$. Since floating point division preserves the relative precision, the absolute error interval will scale proportionally with the value itself.

What ultimately defines the 

The output value will have a precision that 


\subsubsection{Depth buffer representation}

\subsection{Sources of precision errors}
A number of conclusions can be drawn from the previous section:
\begin{itemize}
\item Large translation matrices acting on small vectors or translations may lead to significant loss of precision.
\item Rotation matrices may cause loss of relative precision of small matrix components when applied to matrices with both small and huge numbers. 
\item There is no significant loss of relative precision associated with applying scaling matrices.
\item Projection matrices. TODO.
\item Perspective division. TODO.
\item Integer depths. TODO.
\end{itemize}

\section{Method}

Our method is based on the work of Klashed \etal \cite{KHECY10}. We also use a scenegraph to represent the hierarchical relationships between transformations and objects in a scene but, differently of their work, our method doesn't have a concept of sub-scene and as a result it doesn't require a re-scale process to compensate for perspective effects and mapping of depths to manageable ranges.

\subsection{Precision Adaptive Scene Graph}\label{sec:scenegraph}


In our implementation, the scene graph of a scene is composed of three different types of nodes:
\begin{enumerate}
	\item The root node of the tree representing the whole scene in our 3-d space. It also can be used to represent the center of the universe.
	\item Internal nodes representing an astronomic position \joncomment{Maybe a better name here.} (e.g. the barycenter of a celestial system) or a renderable entity (e.g. a celestial body, spacecraft, etc.).
	\item The leaf nodes representing a renderable entity or the virtual camera of the system.
\end{enumerate}

Each node also stores the transformation matrix (which can be composed by rotation, translation and scale matrices as well) to be applied over the element in the node and a \textbf{node radius}. As usual in a scene graph tree structure, the transformation matrix is inherited by all children of a node. A \textbf{node radius} is defined as a virtual sphere (bounding volume) centered on the node's spatial position which allows one to decide when another node's spatial position is in its influence sphere.

Figure~\ref{fig:scenegraph} illustrates the schematic shape of a scene graph for the scene in the figure. In the top drawing of Figure~\ref{fig:scenegraph} the item number 1 represents the node radius of the scene; item number 2 is another node radius in the scene, with items 4 and 5 inside it plus the virtual camera of the system; item 3 is another celestial body (for the sake of simplicity, let's consider this a heliocentric system so item 3 represents the Sun); item 4 is an orbiting element (this element could be another celestial body, a spacecraft, a satellite, etc.) of item 5, another celestial body. The virtual camera of the system is attached to the item 4 node. This scene graph can be seen as a representation of the Sun in yellow, Earth in blue and Moon in gray. 

\joncomment{Emil, do you think is possible to write some text aside the nodes in the bottom part of Figure~\ref{fig:scenegraph}? Like: badycenter, Moon, Sun, etc?} 
Also in Figure~\ref{fig:scenegraph}, in the bottom right drawing, the representation of the scene as a scene graph tree can be seen. In this example the virtual camera is focusing on item 4, the Moon. We call our scene graph a dynamic scene graph.

\begin{figure}
\includegraphics[width=10cm,height=10cm,keepaspectratio]{scenegraph.pdf}
\caption{(to do)}
\label{fig:scenegraph}
\end{figure}

In order to render the scene correctly, a commonly used scene graph visits all nodes traversing the tree concatenating the transformation matrices throughout the path  from the root node to each of the leaf nodes. In this structure the root of the scene graph is seen as the center of coordinates of the scene (the world coordinates).
Considering the possible differences in size and distance when visualizing objects in universe and the classical way a scene graph is traversed during a scene rendering, this approach can lead the precision problems in the final rendering of the scene, as discussed in section~\ref{sec:theoretical}, mainly because the amount of possible translation transformations performed between different nodes. A higher scene graph may lead to higher errors (\joncomment{This is not always true. I think I will remove this last sentence.}).\joncomment{Should we say more about position and depth and catastrophic cancellation?}

As we noted, traversing the scene graph from root to leaf during each rendering pass will produce a catastrophic cancellation process when the dimensions considered are of order of magnitude. To avoid the catastrophic cancellation process, we traverse the scene graph in a different way than the commonly used way, trying to minimizing the number of transformations executed.\joncomment{TO REVIEW.}

In our dynamic scene graph, the closest (in an euclidean space) node from the camera is the current center of the coordinates system for the scene (center of the world coordinates) and will be called from now on the current center node. In our example in Figure~\ref{fig:scenegraph} this node is the item 4 node (Moon). Thus, instead of applying all translations from the root node to item 4 node, we only apply the translations present in item 4 node, minimizing the chances of a catastrophic cancellation happens.

When visiting other item nodes, for instance, the item 5 node, we initially find the closest ancestor node between the current center node and the item 5 node; the item 2 node. Instead of applying all translations from the root node to item 5 node, we only apply the translation from item 2 node to item 5 node (drawn as a blue arrow in Figure~\ref{fig:scenegraph}, and to obtain the final translation from the current center node, we subtract the translation from the item 2 node to the current center node (drawn as a red arrow in Figure~\ref{fig:scenegraph}). This procedure is equivalent to sum vector $\vec{v_1}$ from item 2 node to item 5 node to vector $\vec{v_2}$ from item 5 node to item 2 node. See Figure~\ref{fig:vectorsum}

The same idea can be extended for the other nodes. For the item 3 node, its position related to the current center node can be obtained adding the vectors $\vec{v_2}$, $\vec{v_3}$ and $\vec{v_4}$ in Figure~\ref{fig:vectorsum}.


\begin{figure}
\includegraphics[width=10cm,height=10cm,keepaspectratio]{vectorsum.pdf}
\caption{(to do)}
\label{fig:vectorsum}
\end{figure}

\joncomment{Talk about the perspective effects.}

We use the perspective matrix:

\begin{equation} \label{eq:ourperspectiveprojection}
P = \mleft(
\begin{array}{cccc}
  \arctan(\frac{\theta_{h}}{2}) & 0 & 0 & 0\\
  0 & \arctan(\frac{\theta_{v}}{2}) & 0 & 0\\
  0 & 0 & 0 & 0\\
  0 & 0 & -1 & 0
\end{array}\mright)
\end{equation}

We manually write the gl\_FragDepth to a floating point buffer using the distance to the fragment computed with the pythagorean theorem.

Clipping is performed for all content with negative w.

\subsubsection{Dynamic camera attachment}

The virtual camera in our system can move freely within 5 degrees of freedom (DOF). Considering only 3 DOF (3-d space movement), the camera can be close to any node in our dynamic scene graph. Which means that once the camera is close to a node, this node must be the new current center node. This propriety of changing the current center node, and by consequence the scene's center of coordinate, based on the camera position is one of the factors contributing to the ``dynamic'' aspect of our scene graph.

In order to determine which node should be the new current center node, every time the camera is translated, its current position is tested against the node radius of the nodes in the dynamic scene graph. If the camera is inside a node's node radius, the camera position is tested against its children nodes' node radius to determine which node should be the new current center node. This idea is represented by the algorithm~\ref{alg:dynamicCamera}.

\begin{algorithm}\label{alg:dynamicCamera}
 \caption{Dynamic Camera Attachment Algorithm}
 \begin{algorithmic}[1]
 \renewcommand{\algorithmicrequire}{\textbf{Input:}}
 \renewcommand{\algorithmicensure}{\textbf{Output:}}
 \REQUIRE in
 \ENSURE  out
 \\ \textit{Initialization} :
  \STATE first statement
 \\ \textit{LOOP Process}
  \FOR {$i = l-2$ to $0$}
  \STATE statements..
  \IF {($i \ne 0$)}
  \STATE statement..
  \ENDIF
  \ENDFOR
 \RETURN $P$
 \end{algorithmic}
\end{algorithm}  

\subsubsection{Dynamic object attachment}

As stated before, not all nodes in the dynamic scene graph are celestial bodies with known orbits. The dynamic scene graph can handle any of the three types defined in section~\ref{sec:scenegraph}. One possible type is a mission space probe. 

Space probes like New Horizons~\cite{??} travels through the solar system capturing important scientific data that is later sent to Earth. Because of New Horizons' trajectory is given by a path from Earth to Pluto, a commonly used scene graph would set New Horizon's node as a child of an existent node in the tree (Earth's node, e.g.) and when the New Horizons probe is rendered close to Pluto, the translations necessary to render that node will lead to an precision problem. Equivalently, setting Pluto's node as the New Horizons' node will generate precision problems when rendering New Horizons probe close to Earth.

The solution to this problem is to have a dynamic node attachment, i.e., during the traveling of New Horizons probe its node changes the parent relationship with the other nodes in the dynamic scene graph. This parent changing (adoption), is done based on the checking of the moving node's position against the other node's node radius. If the moving node is inside a node radius, the children of this node are also tested against to determine the closest one which moving node is inside the node radius. The algorithm~\ref{alg:dynamicAdoption} represents this idea. \joncomment{Improve this paragraph.}

\begin{algorithm}\label{alg:dynamicAdoption}
 \caption{Dynamic Node Attachment Algorithm - Node Adoption}
 \begin{algorithmic}[1]
 \renewcommand{\algorithmicrequire}{\textbf{Input:}}
 \renewcommand{\algorithmicensure}{\textbf{Output:}}
 \REQUIRE in
 \ENSURE  out
 \\ \textit{Initialization} :
  \STATE first statement
 \\ \textit{LOOP Process}
  \FOR {$i = l-2$ to $0$}
  \STATE statements..
  \IF {($i \ne 0$)}
  \STATE statement..
  \ENDIF
  \ENDFOR
 \RETURN $P$
 \end{algorithmic}
\end{algorithm}  

\subsubsection{Model view matrix computation}

How to calculate the final Model View Matrix...

\subsection{Depth Sorting}

\subsection{Volume rendering and Order Independent Transparency}

\section{Result}

\joncomment{OS Scenes?}

%-------------------------------------------------------------------------
\section{Conclusions}


\section{Acknowledgments}



%-------------------------------------------------------------------------

%\bibliographystyle{eg-alpha}
\bibliographystyle{eg-alpha-doi}

\bibliography{dsg-bib}

%-------------------------------------------------------------------------

\end{document}
