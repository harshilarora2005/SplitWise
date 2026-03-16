import { Link, useNavigate, useLocation } from 'react-router-dom'
import { useAuth } from '../context/AuthContext'

export default function Navbar() {
    const { user, logout } = useAuth()
    const navigate = useNavigate()
    const { pathname } = useLocation()

    function handleLogout() {
        logout()
        navigate('/login')
    }

    const isActive = (path) =>
        path === '/dashboard'
            ? pathname === '/dashboard'
            : pathname.startsWith(path)

    return (
        <nav className="sticky top-0 z-50 w-full border-b border-white/ bg-[#13151a]/90 backdrop-blur-xl">
            <div className="w-full px-8">
                <div className="h-16 flex items-center justify-between gap-8">

                    {/* Logo */}
                    <Link
                        to="/dashboard"
                        className="flex items-center gap-2.5 shrink-0 group"
                    >
                        <div className="w-8 h-8 rounded-lg bg-emerald-500/15 border border-emerald-500/25 flex items-center justify-center transition-colors group-hover:bg-emerald-500/20">
                            <span className="text-emerald-400 text-lg font-serif leading-none">÷</span>
                        </div>
                        <span className="font-semibold text-base text-white tracking-tight">
                            splitwise
                        </span>
                    </Link>

                    {/* Nav links */}
                    <div className="flex items-center gap-1 flex-1">
                        {[
                            { to: '/dashboard', label: 'Dashboard' },
                            { to: '/groups', label: 'Groups' },
                        ].map(({ to, label }) => (
                            <Link
                                key={to}
                                to={to}
                                className={`
                                    relative px-4 py-2 rounded-lg text-sm font-medium transition-all duration-150
                                    ${isActive(to)
                                        ? 'text-white bg-white/[0.08]'
                                        : 'text-white/45 hover:text-white/80 hover:bg-white/[0.05]'
                                    }
                                `}
                            >
                                {label}
                                {isActive(to) && (
                                    <span className="absolute -bottom-px left-1/2 -translate-x-1/2 w-4 h-px bg-emerald-400 rounded-full" />
                                )}
                            </Link>
                        ))}
                    </div>

                    {/* Right: user + logout */}
                    <div className="flex items-center gap-2.5 shrink-0 ml-auto">
                        <div className="flex items-center gap-2 px-4 py-2.5 rounded-lg bg-white/[0.04] border border-white/[0.06]">
                            <div className="w-6 h-6 rounded-full bg-emerald-500/20 border border-emerald-500/30 flex items-center justify-center shrink-0">
                                <span className="text-emerald-400 text-[11px] font-semibold uppercase leading-none">
                                    {user?.name?.charAt(0)}
                                </span>
                            </div>
                            <span className="text-sm text-white/60 max-w-[140px] truncate">
                                {user?.name}
                            </span>
                        </div>

                        <button
                            onClick={handleLogout}
                            className="px-5 py-2.5 text-sm font-medium text-white/40 rounded-lg border border-white/[0.08] hover:text-white/75 hover:bg-white/[0.06] hover:border-white/[0.12] transition-all duration-150 active:scale-[0.97]"
                        >
                            Sign out
                        </button>
                    </div>

                </div>
            </div>
        </nav>
    )
}