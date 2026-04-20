%-------------------------
% Resume in Latex
% Author : Arpan Maheshwari
%------------------------

\documentclass[letterpaper,11pt]{article}

\usepackage{latexsym}
\usepackage[empty]{fullpage}
\usepackage{titlesec}
\usepackage{marvosym}
\usepackage[usenames,dvipsnames]{color}
\usepackage{verbatim}
\usepackage{enumitem}
\usepackage[colorlinks=true, urlcolor=blue, linkcolor=blue]{hyperref}
\usepackage{fancyhdr}
\usepackage[english]{babel}
\usepackage{tabularx}
\usepackage{fontawesome}
\input{glyphtounicode}

%----------FONT OPTIONS----------
% sans-serif
% \usepackage[sfdefault]{FiraSans}

\pagestyle{fancy}
\fancyhf{} % clear all header and footer fields
\fancyfoot{}
\renewcommand{\headrulewidth}{0pt}
\renewcommand{\footrulewidth}{0pt}

% Adjust margins for better space utilization
\addtolength{\oddsidemargin}{-0.7in}
\addtolength{\evensidemargin}{-0.7in}
\addtolength{\textwidth}{1.4in}
\addtolength{\topmargin}{-0.95in}
\addtolength{\textheight}{1.65in}

\urlstyle{same}

\raggedbottom
\raggedright
\setlength{\tabcolsep}{0in}

% Sections formatting - tighter spacing
\titleformat{\section}{
  \vspace{-10pt}\scshape\raggedright\large
}{}{0em}{}[\color{black}\titlerule \vspace{-5pt}]

% Ensure that generate pdf is machine readable/ATS parsable
\pdfgentounicode=1

%-------------------------
% Custom commands
\newcommand{\resumeItem}[1]{
  \item\small{
    {#1 \vspace{-2pt}}
  }
}

\newcommand{\resumeSubheading}[4]{
  \vspace{-1pt}\item
    \begin{tabular*}{0.97\textwidth}[t]{l@{\extracolsep{\fill}}r}
      \textbf{#1} & #2 \\
      \textit{\small#3} & \textit{\small #4} \\
    \end{tabular*}\vspace{-5pt}
}

\newcommand{\resumeSubSubheading}[2]{
    \item
    \begin{tabular*}{0.97\textwidth}{l@{\extracolsep{\fill}}r}
      \textit{\small#1} & \textit{\small #2} \\
    \end{tabular*}\vspace{-8pt}
}

\newcommand{\resumeProjectHeading}[2]{
    \item
    \begin{tabular*}{0.97\textwidth}{l@{\extracolsep{\fill}}r}
      \small#1 & #2 \\
    \end{tabular*}\vspace{-8pt}
}

\newcommand{\resumeSubItem}[1]{\resumeItem{#1}\vspace{-8pt}}

\renewcommand\labelitemii{$\vcenter{\hbox{\tiny$\bullet$}}$}

\newcommand{\resumeSubHeadingListStart}{\begin{itemize}[leftmargin=0.11in, label={}]}
\newcommand{\resumeSubHeadingListEnd}{\end{itemize}}
\newcommand{\resumeItemListStart}{\begin{itemize}[leftmargin=0.15in, itemsep=-1pt]}
\newcommand{\resumeItemListEnd}{\end{itemize}\vspace{-8pt}}

%-------------------------------------------
%%%%%%  RESUME STARTS HERE  %%%%%%%%%%%%%%%%%%%%%%%%%%%%

\begin{document}

%----------HEADING----------
\begin{center}
    {\Huge \bfseries Arpan Maheshwari} \\[1pt]
     {\small \textbf{Backend Software Engineer} \textbar{} Java \& Spring Boot} \\[2pt]
    +91 7991377996 \textbar{}
    \href{mailto:maheshwariarpan828@gmail.com}{maheshwariarpan828@gmail.com} \textbar{}
    \href{https://linkedin.com/in/arpan-maheshwari-76879a191/}{LinkedIn} \textbar{}
    \href{https://github.com/ArpanMaheshwari144}{GitHub} \textbar{}
    \href{https://leetcode.com/u/maheshwariarpan828/}{LeetCode}
\end{center}

\section{Professional Summary}
\vspace{-1mm}
\textbf{Backend Software Engineer} building \textbf{Java/Spring Boot 
microservices} for a healthcare platform serving \textbf{10+ countries} 
— delivered \textbf{30\% performance gains} through profiling, resolved 
\textbf{500+ production incidents} (\textbf{92\% closure}, 14 Critical/Blocker, 
110 Major) via log correlation and code + database fixes. Experienced in 
\textbf{REST APIs, Microservices, Redis, Docker, PostgreSQL, AWS}. 
Open to relocation.

%-----------EXPERIENCE-----------
\section{Experience}
\vspace{-1mm}
\resumeSubHeadingListStart
  \resumeSubheading
    {Software Engineer}{Jan 2022 -- Present}
    {Konovo \small{(formerly GlocalMind Inc.) | AI-powered Healthcare Market Research Platform}}{}
    \resumeItemListStart
    % === BUILDING — TOP ===
    \resumeItem{Built and owned the \textbf{vendor-facing portal} 
    end-to-end — designed REST APIs, data models, and integration 
    pipelines for multiple vendors (\textbf{Doximity, M3}) handling 
    survey delivery, completion tracking, and vendor API reconciliation.}
    \resumeItem{Built an \textbf{email deduplication API} using 
    normalization and fuzzy matching — eliminated \textbf{95\% of duplicate}
    physician records and restored cross-platform data consistency.}
    \resumeItem{Migrated the \textbf{GlocalMind platform} from 
    \textbf{JDK 8 to JDK 11} with \textbf{zero downtime} — resolved 
    dependency conflicts and improved throughput by \textbf{15\%}.}
    \resumeItem{Profiled \textbf{15+ Spring Boot microservices} — cut 
    query execution time by \textbf{30\%} through targeted index and 
    pagination fixes.}
    \resumeItem{Expanded \textbf{JUnit test coverage} across Spring Boot 
    microservices and configured \textbf{Jenkins} to email automated test 
    reports — caught regression bugs before production releases.}

    % === DEBUGGING — VARIED VERBS ===
    \resumeItem{Discovered \textbf{AWS API Gateway} silently mis-parsing 
    \textbf{X-Forwarded-For headers} — flagging valid IPs as duplicates 
    across \textbf{300+ vendor integrations}; proved root cause against 
    initial pushback and shipped the fix; recognized by VP and CTO.}
    \resumeItem{Diagnosed a \textbf{MySQL HikariCP connection pool 
    exhaustion} causing \textbf{95\% email delivery failure} 
    (959/1009 dropped) — isolated the blocking batch query, 
    documented root cause, and shipped a targeted fix.}
    \resumeItem{Uncovered a \textbf{MongoDB-MySQL sync gap} silently 
    blocking survey launches — traced the full API call chain, pinpointed 
    the skipped MySQL update, and directly patched \textbf{7 affected 
    surveys}.}
    \resumeItem{Correlated \textbf{CloudWatch, CloudTrail, and New Relic 
    APM} to isolate an \textbf{AWS Lambda concurrency misconfiguration} 
    causing \textbf{1,800+ throttles} and an 8x page load spike — 
    root-cause analysis and fix recognized by VP and CTO.}
    \resumeItem{Debugged a broken \textbf{Doximity automated payment flow} 
    forcing untracked manual payments — fixed root cause in payment logic 
    to restore automated processing; recognized by VP and CTO.}
    \resumeItem{Resolved multiple \textbf{PayPal redemption failures} — 
    traced orphaned credit adjustments causing \textbf{negative balance 
    display} and fixed a \texttt{credit\_status} filtering bug in the 
    payment controller.}

    % === IMPACT SUMMARY ===
    \resumeItem{Owned \textbf{500+ production incidents} across Java, AWS, 
    MySQL, and MongoDB (\textbf{92\% closure}, 14 Critical/Blocker, 110 Major) 
    — investigated via log correlation (\textbf{CloudWatch, Papertrail, New Relic}) 
    and code-level analysis, shipped fixes in code and database schema; 
    mentored a junior engineer and led PR reviews across the team.}
    \resumeItemListEnd
\resumeSubHeadingListEnd

%-----------TECHNICAL SKILLS-----------
\section{Technical Skills}
\vspace{-1mm}
\begin{itemize}[leftmargin=0.4cm, itemsep=-3pt]
    \item \textbf{Languages}: Java, C++, SQL
    \item \textbf{Frameworks}: Spring Boot, Spring MVC, Spring Security, JPA/Hibernate, REST APIs, Microservices
    \item \textbf{Databases}: MySQL, MongoDB, PostgreSQL, Redis, AWS RDS, AWS Aurora, AWS DocumentDB
    \item \textbf{Cloud \& DevOps}: AWS (Lambda, API Gateway, CloudWatch, Step Functions), Docker, Docker Compose, Jenkins, Git, CI/CD
    \item \textbf{Testing \& Tools}: JUnit, Mockito, Maven, Gradle, Kibana, New Relic, Papertrail, Swagger
\end{itemize}

%-----------EDUCATION-----------
\section{Education}
\vspace{-1mm}
  \resumeSubHeadingListStart
    \resumeSubheading
      {Babu Banarasi Das National Institute of Information Technology}{Lucknow, India}
      {Bachelor of Technology, Computer Science and Engineering, CGPA: 7.69}{2017 -- 2021}
  \resumeSubHeadingListEnd

%-----------PROJECTS-----------
\section{Projects}
\vspace{-1mm}
\begin{itemize}[leftmargin=0.4cm, itemsep=-2pt]
    \item \textbf{UnifyPatients:} Doctor networking platform — 
    \textbf{Spring Boot, Neo4J}, JWT auth, real-time messaging, and 
    graph-based connection recommendations for physician discovery 
    and collaboration. [\href{https://app.unifypatients.click/signup}{Live}]
    \item \textbf{PolicyVault:} Insurance management system with 
    \textbf{Spring Boot, MySQL, Docker} — containerized deployment 
    with \textbf{JWT authentication} and optimized REST APIs.[\href{https://github.com/ArpanMaheshwari144/InsuranceManagementSystemUsingSpringBoot}{GitHub}]
\end{itemize}

\section{Achievements}
\vspace{-1mm}
\begin{itemize}[leftmargin=0.4cm, itemsep=-2pt]
    \item \textbf{LeetCode:} 1000+ Day Streak \textbar{} 1000+ 
    Problems Solved \textbar{} Top 10\% Global Ranking
    \item Recognized as \textbf{top performer} in consecutive 
    annual performance reviews
\end{itemize}

%-------------------------------------------
\end{document}
